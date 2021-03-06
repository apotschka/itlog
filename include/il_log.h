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
//  This is a stable class, and may not change except for emergencies. It
//  is provided in stable builds.
#define IL_LOG_USE_LAST 1                   // Log entry supplied last.
#define IL_LOG_USE_AVERAGE 2                // Log average of entries since last log line.
#define IL_LOG_USE_MIN 3                    // Log minimum of entries since last log line.
#define IL_LOG_USE_MAX 4                    // Log maximum of entries since last log line.
#define IL_LOG_USE_SUM 5                    // Log sum of entries since last log line.
#define IL_LOG_UNIT_INTERVAL 0x100          // Flag for special printing of values in [0,1].

//  Constructor.
IL_EXPORT il_log_t *
    il_log_new (void);

//  Destructor.
IL_EXPORT void
    il_log_destroy (il_log_t **self_p);

//  Set the output interval in microseconds. The default is 0.
IL_EXPORT void
    il_log_set_output_interval (il_log_t *self, int msecs);

//  Set the frequency of header lines. The default is 10.
IL_EXPORT void
    il_log_set_header_frequency (il_log_t *self, int freq);

//  Add or update an entry of the logger provided that the print level argument does
//  not exceed the print level of the logger. If entries of the same header name    
//  have been used before, the method accumulates the data and returns false. If no 
//  entry exists yet, the method creates it and returns true. Possible modes for    
//  accumulation of data are                                                        
//                                                                                  
//  * IL_LOG_USE_LAST: Entry supplied last                                          
//  * IL_LOG_USE_AVERAGE: Average of entries since last printing.                   
//  * IL_LOG_USE_MIN: Minimum of entries since last printing.                       
//  * IL_LOG_USE_MAX: Maximum of entries since last printing.                       
//  * IL_LOG_USE_SUM: Sum of entries since last printing.                           
//                                                                                  
//  An additional flag IL_LOG_UNIT_INTERVAL can be set using "|" for special        
//  printing of values between 0 and 1. In this case, the entry format should be    
//  shorter than the header format by three characters.                             
IL_EXPORT bool
    il_log_entry (il_log_t *self, int print_level, const char *header_format, const char *header_name, const char *entry_format, double value, int mode);

//  Set the print level of the logger. All columns exceeding the current print level
//  will not get printed.                                                           
IL_EXPORT void
    il_log_set_print_level (il_log_t *self, int print_level);

//  If the time specified by the output interval has passed since the last time this
//  method has printed a line, the accumulated data will be formatted and printed.  
//  Every 10th line, a newline and a header will be printed (the number can be      
//  changed by setting the header frequency). The accumulated data of all entries   
//  will be reset. If not enough time has passed, nothing happens. The output will  
//  be printed to all files registerd with `il_log_add_file`. Standard output is    
//  registered by default.                                                          
IL_EXPORT void
    il_log_output_line (il_log_t *self);

//  Add another file descriptor to the list of output streams.
IL_EXPORT void
    il_log_add_file (il_log_t *self, FILE *fid);

//  Remove file descriptor from the list of output streams.
IL_EXPORT void
    il_log_remove_file (il_log_t *self, FILE *fid);

//  Usage example and self test.
IL_EXPORT void
    il_log_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
