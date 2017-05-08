
#ifndef _func_track_h
#define _func_track_h
#include <iostream>
#include <stdint.h>
#include <streambuf>
#include <sstream>

extern pthread_key_t func_ctx; //slb
/* FTrack Class -- an FTrack object is created inside every function that will be saved in the
 *    per thread data area.  When the object is instantiated, it grabs the current information
 *    from the running thread and saves it within the object.  This way when the destructor is 
 *    called (when the function returns) it can restore the previous function information for
 *    the previous caller. */ 

class FTrack {
    const char* lfile;
    const char* lfunc;
	class FTrack *prev;

    public:
    /* Constructor -- called on func_entry */
    FTrack(const char* file, const char* func, int line)
    {
        lfile = file;
        lfunc = func;

		prev = (class FTrack*)pthread_getspecific(func_ctx) ;
		pthread_setspecific(func_ctx, (void*)this);
    }

    /* Destructor -- called on func_exit */
    ~FTrack()
    {
		pthread_setspecific(func_ctx, (void*)prev);
    }	

	const char *getfunc() { return lfunc; }
	const char *getfile() { return lfile; }

};
#endif
