#pragma once
#include "stdio.h"
#include "tool.h"

void quick_sort(sq_list* sq);
void q_sort(sq_list* sq, int low, int high);
int partition(sq_list* sq, int low, int high);