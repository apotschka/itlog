/*  =========================================================================
    il_log - Iteration logger class.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of the IterationLogger library.                  
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

#ifndef IL_LOG_H_INCLUDED
#define IL_LOG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @warning THE FOLLOWING @INTERFACE BLOCK IS AUTO-GENERATED BY ZPROJECT
//  @warning Please edit the model at "api/il_log.api" to make changes.
//  @interface
//  This API is a draft, and may change without notice.
#ifdef IL_BUILD_DRAFT_API
#define IL_LOG_USE_LAST 1                   // Log entry supplied last.
#define IL_LOG_USE_AVERAGE 2                // Log average of entries since last log line.
#define IL_LOG_USE_MIN 3                    // Log minimum of entries since last log line.
#define IL_LOG_USE_MAX 4                    // Log maximum of entries since last log line.

//  *** Draft method, for development use, may change without warning ***
//  Constructor.
IL_EXPORT il_log_t *
    il_log_new (void);

//  *** Draft method, for development use, may change without warning ***
//  Destructor.
IL_EXPORT void
    il_log_destroy (il_log_t **self_p);

//  *** Draft method, for development use, may change without warning ***
//  Not implemented yet.
IL_EXPORT void
    il_log_set_print_interval (il_log_t *self, time_t interval);

//  *** Draft method, for development use, may change without warning ***
//  Add or update an entry of the logger. If entries of the same header name
//  have been used before, the method accumulates the data and returns      
//  false. If no entry exists yet, the method creates it and returns true.  
//                                                                          
//  TODO: Describe different modes.                                         
IL_EXPORT bool
    il_log_entry (il_log_t *self, const char *header_format, const char *header_name, const char *entry_format, float value, int mode);

//  *** Draft method, for development use, may change without warning ***
//  If the time specified by the print interval has passed since the last 
//  time this method has produced output, the accumulated data will be    
//  formatted and printed. Every 10th line, a newline and a header will be
//  printed. The accumulated data of all entries will be reset.  If not   
//  enough time has passed, nothing happens.                              
IL_EXPORT void
    il_log_print (il_log_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Usage example and self test.
IL_EXPORT void
    il_log_test (bool verbose);

#endif // IL_BUILD_DRAFT_API
//  @end

#ifdef __cplusplus
}
#endif

#endif