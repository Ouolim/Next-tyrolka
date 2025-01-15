/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Secure Component                                                 */
/**                                                                       */
/**    Datagram Transport Layer Security (DTLS)                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define NX_SECURE_SOURCE_CODE

#include "nx_secure_dtls.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_secure_dtls_psk_add                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function adds a pre-shared key (PSK) to a DTLS session for use */
/*    with a PSK ciphersuite. The second parameter is the PSK identity    */
/*    used during the DTLS handshake to select the proper key.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dtls_session                          Pointer to DTLS Session       */
/*    pre_shared_key                        Pointer to PSK data           */
/*    psk_length                            Length of PSK data            */
/*    psk_identity                          PSK identity data             */
/*    identity_length                       Length of identity data       */
/*    hint                                  PSK hint data                 */
/*    hint_length                           Length of hint data           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nx_secure_tls_psk_add                Add PSK data to TLS           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Timothy Stapko           Initial Version 6.0           */
/*  09-30-2020     Timothy Stapko           Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(NX_SECURE_ENABLE_PSK_CIPHERSUITES) || defined(NX_SECURE_ENABLE_ECJPAKE_CIPHERSUITE)
UINT _nx_secure_dtls_psk_add(NX_SECURE_DTLS_SESSION *dtls_session, UCHAR *pre_shared_key,
                             UINT psk_length, UCHAR *psk_identity, UINT identity_length, UCHAR *hint,
                             UINT hint_length)
{
#ifdef NX_SECURE_ENABLE_DTLS
UINT status;
NX_SECURE_TLS_SESSION *tls_session;

    /* Get the internal TLS session instance. */
    tls_session = &(dtls_session -> nx_secure_dtls_tls_session);

    /* Call TLS API. */
    status = _nx_secure_tls_psk_add(tls_session, pre_shared_key, psk_length, psk_identity, identity_length, hint, hint_length);

    return(status);
#else
    NX_PARAMETER_NOT_USED(dtls_session);
    NX_PARAMETER_NOT_USED(pre_shared_key);
    NX_PARAMETER_NOT_USED(psk_length);
    NX_PARAMETER_NOT_USED(psk_identity);
    NX_PARAMETER_NOT_USED(identity_length);
    NX_PARAMETER_NOT_USED(hint);
    NX_PARAMETER_NOT_USED(hint_length);

    return(NX_NOT_SUPPORTED);
#endif /* NX_SECURE_ENABLE_DTLS */
}
#endif

