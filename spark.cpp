#define LOG_TAG  "WJF_SPARK"
#include <hardware/hardware.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include "spark.h"

#define MODULE_NAME   "SPARK_NAME"
#define MODULE_AUTHOR  "wangjianfeng"
#define SDCARD_REPO_FILE "/sdcard/a"
/*设备打开和关闭接口*/  
static int spark_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);  
static int spark_device_close(struct hw_device_t* device);  
  
/*设备访问接口*/  
static int spark_set_val(spark_device_t* dev, int val);  
static int spark_get_val(spark_device_t* dev, int* val);  
  
/*模块方法表*/  
static struct hw_module_methods_t spark_module_methods = {  
    open: spark_device_open  
}; 


/*模块实例变量*/  
struct spark_module_t HAL_MODULE_INFO_SYM = {  
    common: {  
        tag: HARDWARE_MODULE_TAG,  
        version_major: 1,  
        version_minor: 0,  
        id: SPARK_HARDWARE_MODULE_ID,  
        name: MODULE_NAME,  
        author: MODULE_AUTHOR,  
        methods: &spark_module_methods,  
    }  
};  


static int spark_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device) { 
    spark_device_t* dev;
    dev = (spark_device_t*)malloc(sizeof(spark_device_t));  
      
    if(!dev) {  
        ALOGE("Spark: failed to alloc space");  
        return -EFAULT;  
    }  
  
    memset(dev, 0, sizeof(spark_device_t));  
    dev->common.tag = HARDWARE_DEVICE_TAG;  
    dev->common.version = 0;  
    dev->common.module = (hw_module_t*)module;  
    dev->common.close = spark_device_close;  
    dev->op.set = spark_set_val;
    dev->op.get = spark_get_val;  
  
  
    *device = &(dev->common);  
    sdcard_repo_t *sd_repo=(sdcard_repo_t*)malloc(sizeof(sdcard_repo_t)) ; 
    sd_repo->fd=open(SDCARD_REPO_FILE,O_RDWR) ;
    if(sd_repo->fd < 0){
        ALOGD("Spark: open repo file %s failed.",SDCARD_REPO_FILE);
    }else{
        ALOGD("Spark: open repo file %s success.",SDCARD_REPO_FILE);
    }

    dev->priv=sd_repo;
        

    ALOGI("Spark: open /dev/hello successfully.");  
    return 0;
}

static int spark_device_close(struct hw_device_t* device) {  
    spark_device_t* spark_device = (spark_device_t*)device;  
    sdcard_repo_t *sd_repo=(sdcard_repo_t*)spark_device->priv;
    if(spark_device) {  
        if(sd_repo && sd_repo->fd > 0){
            close(sd_repo->fd);  
        }
        
        if(sd_repo) free(sd_repo); 
        free(spark_device);  
        
        ALOGD("Spark: free spark device successfully.");  
    }  
    ALOGI("Spark:close spark device completed.");  
    return 0;  
}  

static int spark_set_val(spark_device_t* dev, int val) {  
    ALOGI("spark: set value %d to device.", val);  
    sdcard_repo_t *sd_repo=(sdcard_repo_t*)dev->priv;
  
    if(sd_repo && sd_repo->fd > 0){ 
        write(sd_repo->fd, &val, sizeof(val));  
        ALOGI("spark: stored to sdcard.");  
    }
  
    return 0;  
}  
  
static int spark_get_val(spark_device_t* dev, int* val) {  
    if(!val) {  
        ALOGE("spark: error val pointer");  
        return -EFAULT;  
    }  
    
    sdcard_repo_t *sd_repo=(sdcard_repo_t*)dev->priv;
    if(sd_repo && sd_repo->fd > 0){
        read(sd_repo->fd, val, sizeof(*val));  
    }
  
    ALOGI("Spark: get value %d from device", *val);  
  
    return 0;  
}  



