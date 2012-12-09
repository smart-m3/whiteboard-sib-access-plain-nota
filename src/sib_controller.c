/*

  Copyright (c) 2009, Nokia Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.  
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.  
    * Neither the name of Nokia nor the names of its contributors 
    may be used to endorse or promote products derived from this 
    software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */
/*
 * 
 *
 * sib_controller.c
 *
 * Copyright 2007 Nokia Corporation
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib.h>

#include <whiteboard_log.h>

#include "sib_service.h"
#include "sib_controller.h"

#define TEST_UDN "X"
#define TEST_NAME "TestServer"


/*****************************************************************************
 * Structure definitions
 *****************************************************************************/
struct _SIBController
{

  SIBControllerListener listener;
  gpointer listener_data;
  GHashTable *services;
  
  GMutex* mutex; /*locking*/
};



static SIBController* sib_controller_new( );
static void sib_controller_lock(SIBController *self);
static void sib_controller_unlock(SIBController *self);


static guint timeout_id = 0;
static gboolean sib_controller_search_timeout( gpointer user_data);

gint sib_controller_start( SIBController *self);

gint sib_controller_stop( SIBController *self);

static SIBController *controller_singleton = NULL;

static void sib_controller_free_sibserverdata(gpointer data);

/**
 * Create the SIBService singleton if it doesn't exist. Otherwise returns
 * the pointer to the service singleton.
 *
 * @return SIBService*
 */
static SIBController* sib_controller_new( )
{
  whiteboard_log_debug_fb();
  SIBController *self = NULL;
  self = g_new0(SIBController, 1);
  g_return_val_if_fail(self != NULL, NULL);

  self->mutex = g_mutex_new();
  self->listener = NULL;
  self->listener_data = NULL;

  self->services = g_hash_table_new_full(g_str_hash,
					 g_str_equal,
					 NULL, 
					 sib_controller_free_sibserverdata);
  

  whiteboard_log_debug_fe();

  return self;
}

gint sib_controller_set_listener(SIBController *self, SIBControllerListener listener, gpointer user_data)
{
whiteboard_log_debug_fb();
  g_return_val_if_fail( self != NULL, -1);

  self->listener = listener;
  self->listener_data = user_data;
  whiteboard_log_debug_fe();
  return 0;
}


/**
 * Get the SIBService singleton.
 *
 * @return SIBService*
 */
SIBController* sib_controller_get()
{
  whiteboard_log_debug_fb();
  if (controller_singleton == NULL)
    {
      controller_singleton = sib_controller_new();
    }
  whiteboard_log_debug_fe();
  return controller_singleton;
}

/**
 * Destroy a UPnP service
 *
 * @param service The service to destroy
 * @return -1 on errors, 0 on success
 */
gint sib_controller_destroy(SIBController* self)
{

  whiteboard_log_debug_fb();

  g_return_val_if_fail(self != NULL, -1);

  /* Ensure that the ctrl is stopped */
  sib_controller_stop(self);

  g_hash_table_destroy(self->services);
  
  sib_controller_lock(self);

  sib_controller_unlock(self);

  whiteboard_log_debug_fe();

  return 0;
}

/**
 * Lock a SIBController instance during a critical operation
 *
 * @param self The SIBController instance to lock
 */
static void sib_controller_lock(SIBController* self)
{
  g_return_if_fail(self != NULL);
  g_mutex_lock(self->mutex);
}

/**
 * Unlock a SIBController instance after a critical operation
 *
 * @param self The SIBController instance to unlock
 */ 
static void sib_controller_unlock(SIBController* self)
{
  g_return_if_fail(self != NULL);
  g_mutex_unlock(self->mutex);
}


/*****************************************************************************
 *
 *****************************************************************************/

/**
 * Start a controller
 * * @param self The controller to start
 * @return -1 on errors, 0 on success
 */
gint sib_controller_start(SIBController* self)
{
  //  struct timeval tv;
  //const char *version;
  gint retvalue = -1;
  whiteboard_log_debug_fb();
  g_return_val_if_fail(self != NULL, -1);

  retvalue = 0;
  whiteboard_log_debug_fe();

  return retvalue;
}

/**
 * Stop a controller
 *
 * @param controller The controller to stop
 * @return -1 on errors, 0 on success
 */
gint sib_controller_stop(SIBController* controller)
{
  whiteboard_log_debug_fb();

  g_return_val_if_fail(controller != NULL, -1);

  
  
  whiteboard_log_debug_fe();

  return 0;
}


gint sib_controller_search(SIBController *self)
{
  
  whiteboard_log_debug_fb();
  g_return_val_if_fail(self != NULL, -1);

  if( timeout_id == 0)
    {
      timeout_id=g_timeout_add( 5000,   // 5 seconds
				sib_controller_search_timeout,
				self );
    }
  
  
  whiteboard_log_debug_fe();
  return 0;
}

static gboolean sib_controller_search_timeout( gpointer user_data)
{
  SIBController *self = (SIBController *)(user_data);
  SIBService *service = (SIBService *)( self->listener_data);
  SIBServerData *ssdata = NULL;
  whiteboard_log_debug_fb();

  g_return_val_if_fail(self != NULL, FALSE);
  g_return_val_if_fail(service != NULL, FALSE);
  
  ssdata = g_new0(SIBServerData,1);    
  /*   if(servercount == 0) */
/*     { */
/*       ssdata->ip = g_strdup(SIB_IP); */
/*       ssdata->port = SIB_PORT; */
/*       name = g_strdup(TEST_NAME); */
/*       udn = g_strdup(TEST_UDN); */
/*       servercount++; */
/*     } */
/*   else  */
/*     { */
/*       ssdata->ip = g_strdup(SIB_LOCAL_IP); */
/*       ssdata->port = SIB_PORT; */
/*       name = g_strdup(LOCAL_NAME); */
/*       udn = g_strdup(LOCAL_UDN); */
/*       servercount++;  */
/*     } */
  //ssdata->ip = g_strdup(SIB_IP); 
  //   ssdata->port = SIB_PORT; 
       ssdata->name = (guchar *)g_strdup(TEST_NAME); 
       ssdata->uri = (guchar *)g_strdup(TEST_UDN); 
  
       self->listener( SIBDeviceStatusAdded, ssdata, self->listener_data);
       
       sib_controller_free_sibserverdata(ssdata);
       whiteboard_log_debug_fe();
	 
  // do no call again
  return FALSE;
/*   if(servercount > 1) */
/*     return FALSE; */
/*   else */
/*     return TRUE; */
}

static void sib_controller_free_sibserverdata(gpointer data)
{
  SIBServerData *ssdata=(SIBServerData *)data;
  whiteboard_log_debug_fb();
  g_return_if_fail(data!=NULL);
  if(ssdata->uri)
    g_free(ssdata->uri);
  
  if(ssdata->ip)
    g_free(ssdata->ip);

  if(ssdata->name)
    g_free(ssdata->name);
  
  g_free(ssdata);
  whiteboard_log_debug_fe();
}
