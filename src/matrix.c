#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "matrix.h"
#include "error_common.h"

ErrorEnum matrix_init(uint64_t rows, uint64_t columns, float *data,Matrix **m1)
{
    assert(m1!=NULL);
    assert(data!=NULL);
    
    *m1 = malloc(sizeof(Matrix));
    if(*m1 == NULL)  return MALLOC_ERROR;
    
    (*m1)->rows = rows;
    (*m1)->columns = columns;
    (*m1)->data = data;
    return SUCCESS;
}
void matrix_delete(Matrix **m1)
{ 
    assert(m1!=NULL);
    assert(*m1!=NULL);
    
    free((*m1)->data);
    (*m1)->data = NULL;
    free(*m1);
    *m1 = NULL;
    return;
}

ErrorEnum matrix_get_value(uint64_t i, uint64_t j,const Matrix *m1,float *value)
{
    assert(m1!=NULL);
    assert(value!=NULL);

    if (i>= m1->rows ) return OUT_OF_BOUNDS;
    if (j>= m1->columns ) return OUT_OF_BOUNDS;
    
    *value = m1->data[j*m1->rows+i];
    return SUCCESS;
}
ErrorEnum matrix_set_value(uint64_t i, uint64_t j, Matrix *m1,float value)
{
    assert(m1!=NULL);
    
    if (i>= m1->rows ) return OUT_OF_BOUNDS;
    if (j>= m1->columns ) return OUT_OF_BOUNDS;
    
    m1->data[j*m1->rows+i]=value;
    return SUCCESS;
}