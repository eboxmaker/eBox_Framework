/*******************************************************************************
* Copyright (c) 2014 IBM Corp.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* and Eclipse Distribution License v1.0 which accompany this distribution.
*
* The Eclipse Public License is available at
*    http://www.eclipse.org/legal/epl-v10.html
* and the Eclipse Distribution License is available at
*   http://www.eclipse.org/org/documents/edl-v10.php.
*
* Contributors:
*    Ian Craggs - initial API and implementation and/or initial documentation
*******************************************************************************/

#include "ebox.h"
#if !defined(MQTT_LOGGING_H)
 #define MQTT_LOGGING_H

 #define STREAM      stdout
 #if !defined(DEBUG)
  #define DEBUG(...)    \
    { \
        uart1.printf("DEBUG:   %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
        uart1.printf( ## __VA_ARGS__); \
    }
 #endif
 #if !defined(LOG)
  #define LOG(...)    \
    { \
        uart1.printf("LOG:   %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
        uart1.printf( ## __VA_ARGS__); \
    }
 #endif
 #if !defined(WARN)
  #define WARN(...)   \
    { \
        uart1.printf("WARN:  %s L#%d ", __PRETTY_FUNCTION__, __LINE__);  \
        uart1.printf( ## __VA_ARGS__); \
    }
 #endif
 #if !defined(ERROR)
  #define ERROR(...)  \
    { \
        uart1.printf("ERROR: %s L#%d ", __PRETTY_FUNCTION__, __LINE__); \
        uart1.printf( ## __VA_ARGS__); \
    }
 #endif

#endif
