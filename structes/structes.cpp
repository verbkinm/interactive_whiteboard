#include "structes.h"

int getType(QString typeStr)
{
    if( typeStr == "label" )
        return  LABEL;
    else if( typeStr == "clock" )
        return CLOCK;
    else if( typeStr == "date" )
        return DATE;
    else if( typeStr == "run_string" )
        return  RUN_STRING;
    else if( typeStr == "schedule" )
        return  SCHEDULE;
    else if( typeStr == "image_viewer" )
        return IMAGE_VIEWER;
    else if( typeStr == "dont_click" )
        return  DONT_CLICK;
    else if( typeStr == "bells_monitor" )
        return  BELLS_MONITOR;


    return -1;
}
