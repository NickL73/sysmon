/**
 * @file networking.h
 * @author nick
 * @date 6/6/26
 * @brief
 */

#ifndef SYSTEM_MONITOR_NETWORKING_H
#define SYSTEM_MONITOR_NETWORKING_H

#include <stdint.h>

#define NETLIB_ERR_DETAILS_BUF_SZ 128 // Number of bytes available for error details

typedef enum net_code_e
{
    NETLIB_OK              = 0,
    NETLIB_ERR_UNKNOWN     = 1,
    NETLIB_NULL_ARG        = 2,
    NETLIB_ADDRINFO_ERR    = 3,
    NETLIB_CONNECTION_FAIL = 4,
    NETLIB_FCNTL_NB_FAIL   = 5
} net_code_e;

typedef enum net_err_type_e
{
    NETLIB_ERR_TYPE_ERRNO = 0,
    NETLIB_ERR_TYPE_GAI   = 1
} net_err_type_e;

typedef struct netresult_s
{
    net_code_e     code;
    char           details[NETLIB_ERR_DETAILS_BUF_SZ];
    net_err_type_e sys_type;
    int            sys_err;
} netresult_s;

const char * net_error_string(net_code_e code);

netresult_s net_connect(const char * p_host, uint16_t port, int * p_fd_out);


#endif // SYSTEM_MONITOR_NETWORKING_H
