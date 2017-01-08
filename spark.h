#ifndef ANDROID_INCLUDE_SPARK_H
#define ANDROID_INCLUDE_SPARK_H

#include <hardware/hardware.h> 

__BEGIN_DECLS

#define SPARK_HARDWARE_MODULE_ID "SPARK_ID"

struct spark_device_op ;
struct spark_device ;
typedef int (*method_set)(struct spark_device *dev,int val);
typedef int (*method_get)(struct spark_device *dev,int *val);


typedef struct sdcard_repo{
    int fd;
}sdcard_repo_t;

struct spark_device_op{
    method_set set;
    method_get get;
};


typedef struct spark_device{
    struct hw_device_t common;
    struct spark_device_op op;
    void *priv;
}spark_device_t;


struct spark_module_t {  
    struct hw_module_t common;  
};  

__END_DECLS 

#endif
