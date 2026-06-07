/**
 * @file networking.c
 * @author nick
 * @date 6/6/26
 * @brief
 */

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#include "sysmon-networking/networking.h"

#define PORT_STR_LEN 6

/* STATIC FUNCTION DECLARATIONS */

/* PUBLC FUNCTION DEFINITIONS*/
const char * net_error_string(const net_code_e code)
{
    switch (code)
    {
        case NETLIB_OK:
            return "Operation completed successfully.";
        case NETLIB_ERR_UNKNOWN:
            return "Unknown error occurred.";
        case NETLIB_NULL_ARG:
            return "NULL input provided to function.";
        case NETLIB_ADDRINFO_ERR:
            return "Failed to resolve address information (GAI error).";
        case NETLIB_CONNECTION_FAIL:
            return "Failed to establish outgoing connection.";
        case NETLIB_FCNTL_NB_FAIL:
            return "Failed to set file descriptor to non-blocking.";
        default:
            return "Invalid or unrecognized error code.";
    }
}

netresult_s net_connect(const char * p_host, const uint16_t port, int * p_fd_out)
{
    netresult_s result = {.code = NETLIB_ERR_UNKNOWN, .details = {0}, .sys_type = NETLIB_ERR_TYPE_ERRNO, .sys_err = 0};
    int         err    = 0;
    int         fd     = -1;
    int         flags  = -1;
    char        port_str[PORT_STR_LEN] = {0};
    struct addrinfo * p_res            = NULL;
    struct addrinfo * p_check          = NULL;
    struct addrinfo   hints            = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};

    if ((NULL == p_host) || (NULL == p_fd_out))
    {
        result.code = NETLIB_NULL_ARG;
        goto end;
    }

    (void)snprintf(port_str, PORT_STR_LEN, "%d", port);
    err = getaddrinfo(p_host, port_str, &hints, &p_res);
    if (0 != err)
    {
        result.code     = NETLIB_ADDRINFO_ERR;
        result.sys_type = NETLIB_ERR_TYPE_GAI;
        result.sys_err  = err;
        goto end;
    }

    for (p_check = p_res; p_check != NULL; p_check = p_check->ai_next)
    {
        fd = socket(p_check->ai_family, p_check->ai_socktype, p_check->ai_protocol);
        if (0 > fd)
        {
            // socket failed, this might be quit condition, but we'll try again anyway
            continue;
        }

        err = connect(fd, p_check->ai_addr, p_check->ai_addrlen);
        if (-1 != err)
        {
            // Got a valid file descriptor and connected successfully
            break;
        }

        // Not a valid candidate, try again
        close(fd);
    }

    /* Regardless of why the loop is complete, addrinfo is no longer needed */
    freeaddrinfo(p_res);
    p_res = NULL;

    if (NULL == p_check)
    {
        // Unable to make an outbound connection to given host and port
        result.code = NETLIB_CONNECTION_FAIL;
        snprintf(result.details, NETLIB_ERR_DETAILS_BUF_SZ, "Failed to connect to %s:%d", p_host, port);
        goto end;
    }

    // If here, have connection and can set to nonblocking
    flags = fcntl(fd, F_GETFL, 0);
    if (-1 == flags)
    {
        close(fd);
        result.code     = NETLIB_FCNTL_NB_FAIL;
        result.sys_type = NETLIB_ERR_TYPE_ERRNO;
        result.sys_err  = errno;
        goto end;
    }

    err = fcntl(fd, F_SETFL, (flags | O_NONBLOCK));
    if (0 != err)
    {
        close(fd);
        result.code     = NETLIB_FCNTL_NB_FAIL;
        result.sys_type = NETLIB_ERR_TYPE_ERRNO;
        result.sys_err  = errno;
        goto end;
    }

    *p_fd_out   = fd;
    result.code = NETLIB_OK;

end:
    return result;
}
