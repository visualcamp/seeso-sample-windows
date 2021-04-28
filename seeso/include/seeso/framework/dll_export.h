//
// Created by cosge on 2021-03-24.
//

#ifndef SEESO_CORE_WINDOWS_DLL_EXPORT_H
#define SEESO_CORE_WINDOWS_DLL_EXPORT_H


#include <stdarg.h>
#include <stdint.h>

#define LIBRARY_EXPORTS

#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#elif
#    define LIBRARY_API
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct SeeSoObject_ {
  int8_t dummy;
} SeeSoObject;

// TODO: fix integer types

LIBRARY_API const char* GetSeeSoVersionString();

LIBRARY_API int32_t GetSeeSoVersionNumber();

LIBRARY_API SeeSoObject* CreateSeeSo(const char* license_key, uint32_t key_length);

LIBRARY_API void DeleteSeeSo(SeeSoObject* wrapper);


LIBRARY_API int SeeSoInitEyeTracker(SeeSoObject* obj, float camera_fov,
                                    int32_t num_thread, int32_t use_gpu,
                                    const int32_t* status_options, uint32_t status_options_size);

LIBRARY_API int SeeSoDeinitEyeTracker(SeeSoObject* obj);

LIBRARY_API int SeeSoSetTrackingFps(SeeSoObject* obj, int32_t fps);

LIBRARY_API int SeeSoSetFaceDistance(SeeSoObject* obj, int32_t distance_mm);

LIBRARY_API int SeeSoSetFixationCount(SeeSoObject* obj, uint32_t count);

LIBRARY_API int SeeSoSetFilterType(SeeSoObject* obj, int32_t type);

LIBRARY_API int SeeSoSetTargetBoundRegion(SeeSoObject* obj, float left, float top, float right, float bottom);

LIBRARY_API int SeeSoStartCalibration(SeeSoObject* obj, int32_t mode, int32_t criteria, float left, float top, float right, float bottom);

LIBRARY_API int SeeSoStartCollectSamples(SeeSoObject* obj);

LIBRARY_API int SeeSoStopCalibration(SeeSoObject* obj);

LIBRARY_API int SeeSoSetCalibrationData(SeeSoObject* obj, const float *data, uint32_t size);

LIBRARY_API int SeeSoSetCallbackInterface(
    SeeSoObject* seeso_obj, void *callback_obj,
    void(*on_gaze         )(void*, uint64_t, float, float, float, float, int32_t, int32_t, int32_t),
    void(*on_status       )(void*, int32_t, uint64_t, const float* data, uint32_t data_size),
    void(*on_face         )(void*, int32_t, uint64_t, const float* data, uint32_t data_size),
    void(*calib_next_point)(void*, float, float),
    void(*calib_progress  )(void*, float),
    void(*calib_finish    )(void*, const float* data, uint32_t data_size));

LIBRARY_API int SeeSoRemoveCallbackInterface(SeeSoObject* obj);

LIBRARY_API int SeeSoAddFrame(SeeSoObject* obj, int64_t time_stamp, uint8_t *buffer, int32_t width, int32_t height);

LIBRARY_API int SeeSoGetAuthorizationResult(SeeSoObject* obj);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif //SEESO_CORE_WINDOWS_DLL_EXPORT_H
