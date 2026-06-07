/**
 * @file agent.c
 * @author nick
 * @date 6/6/26
 * @brief
 */

#include <stdio.h>

#include "sysmon-networking/networking.h"

/* LOCAL DEFINITIONS AND CONSTANTS */

/* PRIVATE API DECLARATIONS */

int main(void)
{
    int         fd     = 0;
    netresult_s result = net_connect("localhost", 8000, &fd);
    printf("Result: %d; MSG: %s (%s)\n", result.code, net_error_string(result.code), result.details);
    return 0;
}

/* PRIVATE API DEFINITIONS */

/* END OF FILE agent.c */
