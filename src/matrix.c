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
