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
    memcpy(m, &w, sizeof(union er_matrix));
    return ERR_OK;
}

ERAPI er_matrix_translates(er_matrix *m, double tx, double ty, double tz)
{
    ERR ret;
    union er_matrix translate_matrix, result;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    er_matrix_identity(&translate_matrix);
    translate_matrix.values[3][0] = tx;
    translate_matrix.values[3][1] = ty;
    translate_matrix.values[3][2] = tz;
    if ((ret = er_matrix_mulm(&translate_matrix, m, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(union er_matrix));
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
    union er_matrix rotation_matrix = {0}, result;
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
        memcpy(m, &result, sizeof(union er_matrix));
    }
    return ret;
}

ERAPI er_matrix_scale(er_matrix *m, double sx, double sy, double sz)
{
    ERR ret;
    union er_matrix scale_matrix, result;
    if (m == NULL) {
        return ERR_INVALID_ARGS;
    }
    memset(&scale_matrix, 0, sizeof(union er_matrix));
    scale_matrix.values[0][0] = sx;
    scale_matrix.values[1][1] = sy;
    scale_matrix.values[2][2] = sz;
    scale_matrix.values[3][3] = 1.0;
    if ((ret = er_matrix_mulm(&scale_matrix, m, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(union er_matrix));
    }
    return ret;
}

ERAPI er_matrix_ortho(er_matrix *m, double left, double right, double bottom, double top, double near, double far)
{
    ERR ret;
    union er_matrix ortho_matrix = { 0 }, result;
    ortho_matrix.values[0][0] =  2.0 / (right - left);
    ortho_matrix.values[1][1] =  2.0 / (top - bottom);
    ortho_matrix.values[2][2] = -2.0 / (far - near);
    ortho_matrix.values[3][0] = -(right + left) / (right - left);
    ortho_matrix.values[3][1] = -(top + bottom) / (top - bottom);
    ortho_matrix.values[3][3] = 1.0;
    if ((ret = er_matrix_mulm(m, &ortho_matrix, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(union er_matrix));
    }
    return ret;
}

ERAPI er_matrix_perspective(er_matrix *m, double fovy, double aspect, double near, double far)
{
    ERR ret;
    union er_matrix perspective_matrix = { 0 }, result;
    double f = 1.0 / tan(TO_RADS(fovy) / 2.0);
    perspective_matrix.values[0][0] = f / aspect;
    perspective_matrix.values[1][1] = f;
    perspective_matrix.values[2][2] = (far + near) / (near - far);
    perspective_matrix.values[2][3] = -1;
    perspective_matrix.values[3][2] = (2.0 * far * near) / (near - far);
    if ((ret = er_matrix_mulm(m, &perspective_matrix, &result)) == ERR_OK) {
        memcpy(m, &result, sizeof(union er_matrix));
    }
    return ret;
}

ERAPI er_matrix_lookat(er_vector *eye, er_vector *target, er_vector *up, er_matrix *out)
{
    ERR ret = ERR_OK;
    struct er_vector xaxis, yaxis, zaxis;
    double xdot, ydot, zdot;
    union er_matrix view_matrix;
    if (eye == NULL || target == NULL || up == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    er_vector_muls(target, -1);
    memcpy(&zaxis, eye, sizeof(struct er_vector));
    er_vector_addv(&zaxis, target);
    er_vector_normalize(&zaxis);
    er_vector_cross(up, &zaxis, &xaxis);
    er_vector_normalize(&xaxis);
    er_vector_cross(&zaxis, &xaxis, &yaxis);
    er_vector_dot(&xaxis, eye, &xdot);
    er_vector_dot(&yaxis, eye, &ydot);
    er_vector_dot(&zaxis, eye, &zdot);
    view_matrix.values[0][0] = xaxis.i;
    view_matrix.values[0][1] = xaxis.j;
    view_matrix.values[0][2] = xaxis.k;
    view_matrix.values[0][3] = -xdot;
    view_matrix.values[1][0] = yaxis.i;
    view_matrix.values[1][1] = yaxis.j;
    view_matrix.values[1][2] = yaxis.k;
    view_matrix.values[1][3] = -ydot;
    view_matrix.values[2][0] = zaxis.i;
    view_matrix.values[2][1] = zaxis.j;
    view_matrix.values[2][2] = zaxis.k;
    view_matrix.values[2][3] = -zdot;
    view_matrix.values[3][0] = 0;
    view_matrix.values[3][1] = 0;
    view_matrix.values[3][2] = 0;
    view_matrix.values[3][3] = 1;
    memcpy(out, &view_matrix, sizeof(union er_matrix));
    return ret;
}

ERAPI er_matrix_lookatyp(er_vector *eye, double pitch, double yaw, er_matrix *out)
{
    ERR ret = ERR_OK;
    double cos_pitch, sin_pitch;
    double cos_yaw, sin_yaw;
    double xdot, ydot, zdot;
    struct er_vector xaxis, yaxis, zaxis;
    union er_matrix view_matrix;
    if (eye == NULL || out == NULL) {
        return ERR_INVALID_ARGS;
    }
    cos_pitch = cos(TO_RADS(pitch));
    sin_pitch = sin(TO_RADS(pitch));
    cos_yaw = cos(TO_RADS(yaw));
    sin_yaw = sin(TO_RADS(yaw));
    xaxis.i = cos_yaw;
    xaxis.j = -sin_pitch * sin_yaw;
    xaxis.k = -cos_pitch * sin_yaw;
    xaxis.l = 0;
    yaxis.i = 0;
    yaxis.j = cos_pitch;
    yaxis.k = -sin_pitch;
    yaxis.l = 0;
    zaxis.i = sin_yaw;
    zaxis.j = sin_pitch * cos_yaw;
    zaxis.k = cos_pitch * cos_yaw;
    zaxis.l = 0;
    er_vector_dot(&xaxis, eye, &xdot);
    er_vector_dot(&yaxis, eye, &ydot);
    er_vector_dot(&zaxis, eye, &zdot);
    view_matrix.values[0][0] = xaxis.i;
    view_matrix.values[0][1] = xaxis.j;
    view_matrix.values[0][2] = xaxis.k;
    view_matrix.values[0][3] = -xdot;
    view_matrix.values[1][0] = yaxis.i;
    view_matrix.values[1][1] = yaxis.j;
    view_matrix.values[1][2] = yaxis.k;
    view_matrix.values[1][3] = -ydot;
    view_matrix.values[2][0] = zaxis.i;
    view_matrix.values[2][1] = zaxis.j;
    view_matrix.values[2][2] = zaxis.k;
    view_matrix.values[2][3] = -zdot;
    view_matrix.values[3][0] = 0;
    view_matrix.values[3][1] = 0;
    view_matrix.values[3][2] = 0;
    view_matrix.values[3][3] = 1;
    memcpy(out, &view_matrix, sizeof(union er_matrix));
    return ret;
}

ERAPI er_matrix_print(er_matrix *m)
{
    if (m == NULL) {
        LOGE("Matrix to log is NULL\n");
        return ERR_INVALID_ARGS;
    }
    LOGI("= Logging matrix =\n");
    LOGI("[ %f,  %f,  %f,  %f ]\n", m->values[0][0], m->values[1][0], m->values[2][0], m->values[3][0]);
    LOGI("[ %f,  %f,  %f,  %f ]\n", m->values[0][1], m->values[1][1], m->values[2][1], m->values[3][1]);
    LOGI("[ %f,  %f,  %f,  %f ]\n", m->values[0][2], m->values[1][2], m->values[2][2], m->values[3][2]);
    LOGI("[ %f,  %f,  %f,  %f ]\n", m->values[0][3], m->values[1][3], m->values[2][3], m->values[3][3]);
    return ERR_OK;
}

static int mstack_empty(er_matrix_stack stack)
{
    return stack->top < 0;
}

static int mstack_full(er_matrix_stack stack)
{
    return stack->top >= stack->capacity - 1;
}

static ERAPI mstack_grow(er_matrix_stack stack)
{
    if (stack == NULL) {
        return ERR_INVALID_ARGS;
    }
    stack->matrices = er__realloc(stack->matrices, sizeof(union er_matrix) * stack->capacity * 2);
    if (stack->matrices == NULL) {
        return ERR_MEMORY_ERROR;
    }
    stack->capacity *= 2;
    return ERR_OK;
}

ERAPI er_mstack_init(er_matrix_stack *stack)
{
    if (stack == NULL) {
        return ERR_INVALID_ARGS;
    }
    *stack = er__malloc(sizeof(struct er_matrix_stack));
    if (*stack == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*stack)->top = -1;
    (*stack)->capacity = 128;
    (*stack)->matrices = er__malloc(sizeof(union er_matrix) * 128);
    memset((*stack)->matrices, 0, sizeof(union er_matrix) * 128);
    return ERR_OK;
}

ERAPI er_mstack_destroy(er_matrix_stack *stack)
{
    if (stack == NULL || *stack == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*stack)->matrices != NULL) {
        er__free((*stack)->matrices);
    }
    er__free(*stack);
    *stack = NULL;
    return ERR_OK;
}

ERAPI er_mstack_pop(er_matrix_stack *stack)
{
    if (stack == NULL || *stack == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (mstack_empty(*stack)) {
        LOGE("Can't pop any more matrices from this stack\n");
        return ERR_UNKNOWN;
    }
    (*stack)->top -= 1;
    return ERR_OK;
}

ERAPI er_mstack_push(er_matrix_stack *stack)
{
    ERR ret;
    if (stack == NULL || *stack == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (mstack_full(*stack)) {
        if ((ret = mstack_grow(*stack)) != ERR_OK) {
            return ret;
        }
    }
    if (mstack_empty(*stack)) {
        (*stack)->top = 0;
        memset((*stack)->matrices, 0, sizeof(union er_matrix));
    } else {
        memcpy((*stack)->matrices + (*stack)->top + 1, (*stack)->matrices + (*stack)->top, sizeof(union er_matrix));
        (*stack)->top += 1;
    }
    return ERR_OK;
}

ERAPI er_mstack_peek(er_matrix_stack *stack, er_matrix **top)
{
    if (stack == NULL || *stack == NULL || top == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (mstack_empty(*stack)) {
        LOGE("Can't peek at an empty stack\n");
        return ERR_UNKNOWN;
    }
    *top = ((*stack)->matrices) + (*stack)->top;
    return ERR_OK;
}
