#include "texgen.h"

GLenum texGenS;
GLfloat texGenSv[4];

GLenum texGenT;
GLfloat texGenTv[4];

void glTexGeni(GLenum coord, GLenum pname, GLint param) {
    // coord is in: GL_S, GL_T, GL_R, GL_Q
    // pname == GL_TEXTURE_GEN_MODE
    /* param is in:
        GL_OBJECT_LINEAR, GL_EYE_LINEAR,
        GL_SPHERE_MAP, GL_NORMAL_MAP, or GL_REFLECTION_MAP
    */
    switch (coord) {
        case GL_S: state.texgen.S = param; break;
        case GL_T: state.texgen.T = param; break;
    }
}

void glTexGenfv(GLenum coord, GLenum pname, GLfloat *param) {
    // pname is in: GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE, GL_EYE_PLANE

    if (pname == GL_TEXTURE_GEN_MODE) {
        switch (coord) {
            case GL_S: state.texgen.S = *param; break;
            case GL_T: state.texgen.T = *param; break;
        }
    } else {
        switch (coord) {
            case GL_S:
                memcpy(texGenSv, param, 4 * sizeof(GLfloat));
                break;
            case GL_T:
                memcpy(texGenTv, param, 4 * sizeof(GLfloat));
                break;
        }
    }

    /*
    If pname is GL_TEXTURE_GEN_MODE, then the array must contain
    a single symbolic constant, one of
    GL_OBJECT_LINEAR, GL_EYE_LINEAR, GL_SPHERE_MAP, GL_NORMAL_MAP,
    or GL_REFLECTION_MAP.
    Otherwise, params holds the coefficients for the texture-coordinate
    generation function specified by pname.
    */
}

GLfloat dot(GLfloat *a, GLfloat *b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

GLfloat gen_tex_coord(GLfloat *vert, GLenum type, GLfloat *params) {
    switch (type) {
        case GL_OBJECT_LINEAR:
        case GL_SPHERE_MAP:
            return dot(vert, params);
            break;
    }
    return 0;
}

void gen_tex_coords(GLfloat *verts, GLfloat **coords, GLint count) {
    *coords = (GLfloat *)malloc(count * 2 * sizeof(GLfloat));
    int i;
    for (i = 0; i < count; i++) {
        GLfloat *tex = &(*coords)[i];
        if (state.enable.texgen_s)
            tex[0] = gen_tex_coord(&verts[i], state.texgen.S, state.texgen.Sv);

        if (state.enable.texgen_t)
            tex[1] = gen_tex_coord(&verts[i], state.texgen.T, state.texgen.Tv);
    }
}
