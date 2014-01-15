#include "internal.h"

ERAPI er_vector_addv(er_vector *v1, er_vector *v2)
{
    if (v1 == NULL || v2 == NULL) {
        return ERR_INVALID_ARGS;
    }
    v1->i += v2->i;
    v1->j += v2->j;
    v1->k += v2->k;
    v1->l += v2->l;
    return ERR_OK;
}

ERAPI er_vector_adds(er_vector *v, double s)
{
    if (v == NULL) {
        return ERR_INVALID_ARGS;
    }
    v->i += s;
    v->j += s;
    v->k += s;
    v->l += s;
    return ERR_OK;
}

ERAPI er_vector_muls(er_vector *v, double s)
{
    if (v == NULL) {
        return ERR_INVALID_ARGS;
    }
    v->i *= s;
    v->j *= s;
    v->k *= s;
    v->l *= s;
    return ERR_OK;
}

ERAPI er_vector_dot(er_vector *v1, er_vector *v2, double *out)
{
    if (v1 == NULL || v2 == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    *out = v1->i * v2->i + v1->j * v2->j + v1->k * v2->k + v1->l * v2->l;
    return ERR_OK;
}

ERAPI er_vector_cross(er_vector *v1, er_vector *v2, er_vector *out)
{
    if (v1 == NULL || v2 == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    out->i = v1->j * v2->k - v1->k * v2->j;
    out->j = v1->k * v2->i - v1->i * v2->k;
    out->k = v1->i * v2->j - v1->j * v2->i;
    out->l = 0;
    return ERR_OK;
}

ERAPI er_vector_mag(er_vector *v, double *out)
{
    if (v == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    *out = sqrt(v->i * v->i + v->j * v->j + v->k * v->k + v->l * v->l);
    return ERR_OK;
}

ERAPI er_vector_normalize(er_vector *v)
{
    double mag;
    if (v == NULL || er_vector_mag(v, &mag) != ERR_OK) {
        return ERR_INVALID_ARGS;
    }
    v->i /= mag;
    v->j /= mag;
    v->k /= mag;
    v->l /= mag;
    return ERR_OK;
}

ERAPI er_matrix_addm(er_matrix *m1, er_matrix *m2)
{
    if (m1 == NULL || m2 == NULL) {
        return ERR_INVALID_ARGS;
    }
    m1->vectors[0].i += m2->vectors[0].i;
    m1->vectors[0].j += m2->vectors[0].j;
    m1->vectors[0].k += m2->vectors[0].k;
    m1->vectors[0].l += m2->vectors[0].l;
    m1->vectors[1].i += m2->vectors[1].i;
    m1->vectors[1].j += m2->vectors[1].j;
    m1->vectors[1].k += m2->vectors[1].k;
    m1->vectors[1].l += m2->vectors[1].l;
    m1->vectors[2].i += m2->vectors[2].i;
    m1->vectors[2].j += m2->vectors[2].j;
    m1->vectors[2].k += m2->vectors[2].k;
    m1->vectors[2].l += m2->vectors[2].l;
    m1->vectors[3].i += m2->vectors[3].i;
    m1->vectors[3].j += m2->vectors[3].j;
    m1->vectors[3].k += m2->vectors[3].k;
    m1->vectors[3].l += m2->vectors[3].l;
    return ERR_OK;
}

ERAPI er_matrix_adds(er_matrix *m, double s)
{
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    m->vectors[0].i += s;
    m->vectors[0].k += s;
    m->vectors[0].j += s;
    m->vectors[0].k += s;
    m->vectors[1].i += s;
    m->vectors[1].k += s;
    m->vectors[1].j += s;
    m->vectors[1].k += s;
    m->vectors[2].i += s;
    m->vectors[2].k += s;
    m->vectors[2].j += s;
    m->vectors[2].k += s;
    m->vectors[3].i += s;
    m->vectors[3].k += s;
    m->vectors[3].j += s;
    m->vectors[3].k += s;
    return ERR_OK;
}

ERAPI er_matrix_mulm(er_matrix *m1, er_matrix *m2, er_matrix *out)
{
    if (m1 == NULL || m2 == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    out->vectors[0].i = m1->vectors[0].i * m2->vectors[0].i
                      + m1->vectors[1].i * m2->vectors[0].j
                      + m1->vectors[2].i * m2->vectors[0].k
                      + m1->vectors[3].i * m2->vectors[0].l;
    out->vectors[0].j = m1->vectors[0].j * m2->vectors[0].i
                      + m1->vectors[1].j * m2->vectors[0].j
                      + m1->vectors[2].j * m2->vectors[0].k
                      + m1->vectors[3].j * m2->vectors[0].l;
    out->vectors[0].k = m1->vectors[0].k * m2->vectors[0].i
                      + m1->vectors[1].k * m2->vectors[0].j
                      + m1->vectors[2].k * m2->vectors[0].k
                      + m1->vectors[3].k * m2->vectors[0].l;
    out->vectors[0].l = m1->vectors[0].l * m2->vectors[0].i
                      + m1->vectors[1].l * m2->vectors[0].j
                      + m1->vectors[2].l * m2->vectors[0].k
                      + m1->vectors[3].l * m2->vectors[0].l;
    out->vectors[1].i = m1->vectors[0].i * m2->vectors[1].i
                      + m1->vectors[1].i * m2->vectors[1].j
                      + m1->vectors[2].i * m2->vectors[1].k
                      + m1->vectors[3].i * m2->vectors[1].l;
    out->vectors[1].j = m1->vectors[0].j * m2->vectors[1].i
                      + m1->vectors[1].j * m2->vectors[1].j
                      + m1->vectors[2].j * m2->vectors[1].k
                      + m1->vectors[3].j * m2->vectors[1].l;
    out->vectors[1].k = m1->vectors[0].k * m2->vectors[1].i
                      + m1->vectors[1].k * m2->vectors[1].j
                      + m1->vectors[2].k * m2->vectors[1].k
                      + m1->vectors[3].k * m2->vectors[1].l;
    out->vectors[1].l = m1->vectors[0].l * m2->vectors[1].i
                      + m1->vectors[1].l * m2->vectors[1].j
                      + m1->vectors[2].l * m2->vectors[1].k
                      + m1->vectors[3].l * m2->vectors[1].l;
    out->vectors[2].i = m1->vectors[0].i * m2->vectors[2].i
                      + m1->vectors[1].i * m2->vectors[2].j
                      + m1->vectors[2].i * m2->vectors[2].k
                      + m1->vectors[3].i * m2->vectors[2].l;
    out->vectors[2].j = m1->vectors[0].j * m2->vectors[2].i
                      + m1->vectors[1].j * m2->vectors[2].j
                      + m1->vectors[2].j * m2->vectors[2].k
                      + m1->vectors[3].j * m2->vectors[2].l;
    out->vectors[2].k = m1->vectors[0].k * m2->vectors[2].i
                      + m1->vectors[1].k * m2->vectors[2].j
                      + m1->vectors[2].k * m2->vectors[2].k
                      + m1->vectors[3].k * m2->vectors[2].l;
    out->vectors[2].l = m1->vectors[0].l * m2->vectors[2].i
                      + m1->vectors[1].l * m2->vectors[2].j
                      + m1->vectors[2].l * m2->vectors[2].k
                      + m1->vectors[3].l * m2->vectors[2].l;
    out->vectors[3].i = m1->vectors[0].i * m2->vectors[3].i
                      + m1->vectors[1].i * m2->vectors[3].j
                      + m1->vectors[2].i * m2->vectors[3].k
                      + m1->vectors[3].i * m2->vectors[3].l;
    out->vectors[3].j = m1->vectors[0].j * m2->vectors[3].i
                      + m1->vectors[1].j * m2->vectors[3].j
                      + m1->vectors[2].j * m2->vectors[3].k
                      + m1->vectors[3].j * m2->vectors[3].l;
    out->vectors[3].k = m1->vectors[0].k * m2->vectors[3].i
                      + m1->vectors[1].k * m2->vectors[3].j
                      + m1->vectors[2].k * m2->vectors[3].k
                      + m1->vectors[3].k * m2->vectors[3].l;
    out->vectors[3].l = m1->vectors[0].l * m2->vectors[3].i
                      + m1->vectors[1].l * m2->vectors[3].j
                      + m1->vectors[2].l * m2->vectors[3].k
                      + m1->vectors[3].l * m2->vectors[3].l;
    return ERR_OK;
}

ERAPI er_matrix_muls(er_matrix *m, double s)
{
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    m->vectors[0].i *= s;
    m->vectors[0].j *= s;
    m->vectors[0].k *= s;
    m->vectors[0].l *= s;
    m->vectors[1].i *= s;
    m->vectors[1].j *= s;
    m->vectors[1].k *= s;
    m->vectors[1].l *= s;
    m->vectors[2].i *= s;
    m->vectors[2].j *= s;
    m->vectors[2].k *= s;
    m->vectors[2].l *= s;
    m->vectors[3].i *= s;
    m->vectors[3].j *= s;
    m->vectors[3].k *= s;
    m->vectors[3].l *= s;
    return ERR_OK;
}

ERAPI er_matrix_identity(er_matrix *m)
{
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    m->vectors[0].i = 1;
    m->vectors[0].j = 0;
    m->vectors[0].k = 0;
    m->vectors[0].l = 0;
    m->vectors[1].i = 0;
    m->vectors[1].j = 1;
    m->vectors[1].k = 0;
    m->vectors[1].l = 0;
    m->vectors[2].i = 0;
    m->vectors[2].j = 0;
    m->vectors[2].k = 1;
    m->vectors[2].l = 0;
    m->vectors[3].i = 0;
    m->vectors[3].j = 0;
    m->vectors[3].k = 0;
    m->vectors[3].l = 1;
    return ERR_OK;
}

ERAPI er_matrix_determinant(er_matrix *m, double *out)
{
    if (m == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    *out = m->values[0][3] * m->values[1][2] * m->values[2][1] * m->values[3][0] -
           m->values[0][2] * m->values[1][3] * m->values[2][1] * m->values[3][0] -
           m->values[0][3] * m->values[1][1] * m->values[2][2] * m->values[3][0] +
           m->values[0][1] * m->values[1][3] * m->values[2][2] * m->values[3][0] +
           m->values[0][2] * m->values[1][1] * m->values[2][3] * m->values[3][0] -
           m->values[0][1] * m->values[1][2] * m->values[2][3] * m->values[3][0] -
           m->values[0][3] * m->values[1][2] * m->values[2][0] * m->values[3][1] +
           m->values[0][2] * m->values[1][3] * m->values[2][0] * m->values[3][1] +
           m->values[0][3] * m->values[1][0] * m->values[2][2] * m->values[3][1] -
           m->values[0][0] * m->values[1][3] * m->values[2][2] * m->values[3][1] -
           m->values[0][2] * m->values[1][0] * m->values[2][3] * m->values[3][1] +
           m->values[0][0] * m->values[1][2] * m->values[2][3] * m->values[3][1] +
           m->values[0][3] * m->values[1][1] * m->values[2][0] * m->values[3][2] -
           m->values[0][1] * m->values[1][3] * m->values[2][0] * m->values[3][2] -
           m->values[0][3] * m->values[1][0] * m->values[2][1] * m->values[3][2] +
           m->values[0][0] * m->values[1][3] * m->values[2][1] * m->values[3][2] +
           m->values[0][1] * m->values[1][0] * m->values[2][3] * m->values[3][2] -
           m->values[0][0] * m->values[1][1] * m->values[2][3] * m->values[3][2] -
           m->values[0][2] * m->values[1][1] * m->values[2][0] * m->values[3][3] +
           m->values[0][1] * m->values[1][2] * m->values[2][0] * m->values[3][3] +
           m->values[0][2] * m->values[1][0] * m->values[2][1] * m->values[3][3] -
           m->values[0][0] * m->values[1][2] * m->values[2][1] * m->values[3][3] -
           m->values[0][1] * m->values[1][0] * m->values[2][2] * m->values[3][3] +
           m->values[0][0] * m->values[1][1] * m->values[2][2] * m->values[3][3];
    return ERR_OK;
}

ERAPI er_matrix_inverse(er_matrix *m)
{
    er_matrix w;
    double det;
    int i, j;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    w.values[0][0] =  m->values[1][1] * m->values[2][2] * m->values[3][3] -
                      m->values[1][1] * m->values[3][2] * m->values[2][3] -
                      m->values[1][2] * m->values[2][1] * m->values[3][3] +
                      m->values[1][2] * m->values[3][1] * m->values[2][3] +
                      m->values[1][3] * m->values[2][1] * m->values[3][2] -
                      m->values[1][3] * m->values[3][1] * m->values[2][2];
    w.values[0][1] = -m->values[0][1] * m->values[2][2] * m->values[3][3] +
                      m->values[0][1] * m->values[3][2] * m->values[2][3] +
                      m->values[0][2] * m->values[2][1] * m->values[3][3] -
                      m->values[0][2] * m->values[3][1] * m->values[2][3] -
                      m->values[0][3] * m->values[2][1] * m->values[3][2] +
                      m->values[0][3] * m->values[3][1] * m->values[2][2];
    w.values[0][2] =  m->values[0][1] * m->values[1][2] * m->values[3][3] -
                      m->values[0][1] * m->values[3][2] * m->values[1][3] -
                      m->values[0][2] * m->values[1][1] * m->values[3][3] +
                      m->values[0][2] * m->values[3][1] * m->values[1][3] +
                      m->values[0][3] * m->values[1][1] * m->values[3][2] -
                      m->values[0][3] * m->values[3][1] * m->values[1][2];
    w.values[0][3] = -m->values[0][1] * m->values[1][2] * m->values[2][3] +
                      m->values[0][1] * m->values[2][2] * m->values[1][3] +
                      m->values[0][2] * m->values[1][1] * m->values[2][3] -
                      m->values[0][2] * m->values[2][1] * m->values[1][3] -
                      m->values[0][3] * m->values[1][1] * m->values[2][2] +
                      m->values[0][3] * m->values[2][1] * m->values[1][2];
    w.values[1][0] = -m->values[1][0] * m->values[2][2] * m->values[3][3] +
                      m->values[1][0] * m->values[3][2] * m->values[2][3] +
                      m->values[1][2] * m->values[2][0] * m->values[3][3] -
                      m->values[1][2] * m->values[3][0] * m->values[2][3] -
                      m->values[1][3] * m->values[2][0] * m->values[3][2] +
                      m->values[1][3] * m->values[3][0] * m->values[2][2];
    w.values[1][1] =  m->values[0][0] * m->values[2][2] * m->values[3][3] -
                      m->values[0][0] * m->values[3][2] * m->values[2][3] -
                      m->values[0][2] * m->values[2][0] * m->values[3][3] +
                      m->values[0][2] * m->values[3][0] * m->values[2][3] +
                      m->values[0][3] * m->values[2][0] * m->values[3][2] -
                      m->values[0][3] * m->values[3][0] * m->values[2][2];
    w.values[1][2] = -m->values[0][0] * m->values[1][2] * m->values[3][3] +
                      m->values[0][0] * m->values[3][2] * m->values[1][3] +
                      m->values[0][2] * m->values[1][0] * m->values[3][3] -
                      m->values[0][2] * m->values[3][0] * m->values[1][3] -
                      m->values[0][3] * m->values[1][0] * m->values[3][2] +
                      m->values[0][3] * m->values[3][0] * m->values[1][2];
    w.values[1][2] =  m->values[0][0] * m->values[1][2] * m->values[2][3] -
                      m->values[0][0] * m->values[2][2] * m->values[1][3] -
                      m->values[0][2] * m->values[1][0] * m->values[2][3] +
                      m->values[0][2] * m->values[2][0] * m->values[1][3] +
                      m->values[0][3] * m->values[1][0] * m->values[2][2] -
                      m->values[0][3] * m->values[2][0] * m->values[1][2];
    w.values[2][0] =  m->values[1][0] * m->values[2][1] * m->values[3][3] -
                      m->values[1][0] * m->values[3][1] * m->values[2][3] -
                      m->values[1][1] * m->values[2][0] * m->values[3][3] +
                      m->values[1][1] * m->values[3][0] * m->values[2][3] +
                      m->values[1][3] * m->values[2][0] * m->values[3][1] -
                      m->values[1][3] * m->values[3][0] * m->values[2][1];
    w.values[2][1] = -m->values[0][0] * m->values[2][1] * m->values[3][3] +
                      m->values[0][0] * m->values[3][1] * m->values[2][3] +
                      m->values[0][1] * m->values[2][0] * m->values[3][3] -
                      m->values[0][1] * m->values[3][0] * m->values[2][3] -
                      m->values[0][3] * m->values[2][0] * m->values[3][1] +
                      m->values[0][3] * m->values[3][0] * m->values[2][1];
    w.values[2][2] =  m->values[0][0] * m->values[1][1] * m->values[3][3] -
                      m->values[0][0] * m->values[3][1] * m->values[1][3] -
                      m->values[0][1] * m->values[1][0] * m->values[3][3] +
                      m->values[0][1] * m->values[3][0] * m->values[1][3] +
                      m->values[0][3] * m->values[1][0] * m->values[3][1] -
                      m->values[0][3] * m->values[3][0] * m->values[1][1];
    w.values[2][2] = -m->values[0][0] * m->values[1][1] * m->values[2][3] +
                      m->values[0][0] * m->values[2][1] * m->values[1][3] +
                      m->values[0][1] * m->values[1][0] * m->values[2][3] -
                      m->values[0][1] * m->values[2][0] * m->values[1][3] -
                      m->values[0][3] * m->values[1][0] * m->values[2][1] +
                      m->values[0][3] * m->values[2][0] * m->values[1][1];
    w.values[3][0] = -m->values[1][0] * m->values[2][1] * m->values[3][2] +
                      m->values[1][0] * m->values[3][1] * m->values[2][2] +
                      m->values[1][1] * m->values[2][0] * m->values[3][2] -
                      m->values[1][1] * m->values[3][0] * m->values[2][2] -
                      m->values[1][2] * m->values[2][0] * m->values[3][1] +
                      m->values[1][2] * m->values[3][0] * m->values[2][1];
    w.values[3][1] =  m->values[0][0] * m->values[2][1] * m->values[3][2] -
                      m->values[0][0] * m->values[3][1] * m->values[2][2] -
                      m->values[0][1] * m->values[2][0] * m->values[3][2] +
                      m->values[0][1] * m->values[3][0] * m->values[2][2] +
                      m->values[0][2] * m->values[2][0] * m->values[3][1] -
                      m->values[0][2] * m->values[3][0] * m->values[2][1];
    w.values[3][2] = -m->values[0][0] * m->values[1][1] * m->values[3][2] +
                      m->values[0][0] * m->values[3][1] * m->values[1][2] +
                      m->values[0][1] * m->values[1][0] * m->values[3][2] -
                      m->values[0][1] * m->values[3][0] * m->values[1][2] -
                      m->values[0][2] * m->values[1][0] * m->values[3][1] +
                      m->values[0][2] * m->values[3][0] * m->values[1][1];
    w.values[3][3] =  m->values[0][0] * m->values[1][1] * m->values[2][2] -
                      m->values[0][0] * m->values[2][1] * m->values[1][2] -
                      m->values[0][1] * m->values[1][0] * m->values[2][2] +
                      m->values[0][1] * m->values[2][0] * m->values[1][2] +
                      m->values[0][2] * m->values[1][0] * m->values[2][1] -
                      m->values[0][2] * m->values[2][0] * m->values[1][1];
    det = m->values[0][0] * w.values[0][0] +
          m->values[1][0] * w.values[0][1] +
          m->values[2][0] * w.values[0][2] +
          m->values[3][0] * w.values[0][3];
    if (det == 0) {
        return 0;
    }
    det = 1.0 / det;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            w.values[i][j] *= det;
        }
    }
    memcpy(m, &w, sizeof(struct er_matrix));
    return ERR_OK;
}

ERAPI er_matrix_translates(er_matrix *m, double tx, double ty, double tz)
{
    ERR ret;
    struct er_matrix translate_matrix, result;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    er_matrix_identity(&translate_matrix);
    translate_matrix.values[3][0] = tx;
    translate_matrix.values[3][1] = ty;
    translate_matrix.values[3][2] = tz;
    if ((ret = er_matrix_mulm(&translate_matrix, m, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(struct er_matrix));
    }
    return ret;
}

ERAPI er_matrix_translatev(er_matrix *m, er_vector *v)
{
    if (m == NULL || v == NULL) {
        return ERR_INVALID_ARGS;
    }
    return er_matrix_translates(m, v->i, v->j, v->k);
}

ERAPI er_matrix_rotate(er_matrix *m, double angle, double x, double y, double z)
{
    ERR ret;
    struct er_matrix rotation_matrix = {0}, result;
    double c = 0, s = 0;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    c = cos(TO_RADS(angle));
    s = sin(TO_RADS(angle));
    rotation_matrix.values[0][0] = x*x*(1-c)+c;
    rotation_matrix.values[0][1] = y*x*(1-c)+z*s;
    rotation_matrix.values[0][2] = x*z*(1-c)-y*s;
    rotation_matrix.values[1][0] = x*y*(1-c)-z*s;
    rotation_matrix.values[1][1] = y*y*(1-c)+c;
    rotation_matrix.values[1][2] = y*z*(1-c)+x*s;
    rotation_matrix.values[2][0] = x*z*(1-c)+y*s;
    rotation_matrix.values[2][1] = y*z*(1-c)-x*s;
    rotation_matrix.values[2][2] = z*z*(1-c)+c;
    rotation_matrix.values[3][3] = 1;
    if ((ret = er_matrix_mulm(&rotation_matrix, m, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(struct er_matrix));
    }
    return ret;
}

ERAPI er_matrix_scale(er_matrix *m, double sx, double sy, double sz)
{
    ERR ret;
    struct er_matrix scale_matrix, result;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    memset(&scale_matrix, 0, sizeof(struct er_matrix));
    scale_matrix.values[0][0] = sx;
    scale_matrix.values[1][1] = sy;
    scale_matrix.values[2][2] = sz;
    scale_matrix.values[3][3] = 1.0;
    if ((ret = er_matrix_mulm(&scale_matrix, m, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(struct er_matrix));
    }
    return ret;
}
