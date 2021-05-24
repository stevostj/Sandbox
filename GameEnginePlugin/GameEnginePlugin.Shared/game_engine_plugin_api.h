#ifndef _GEPAPI_H_
#define _GEPAPI_H_

#ifdef _EXPORTGEP
#define GEP_DECLSPEC    __declspec(dllexport)
#else
#define GEP_DECLSPEC    __declspec(dllimport)
#endif

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

#define GEP_INITIALIZE_OK 0

GEP_DECLSPEC int GEP_Initialize();

GEP_DECLSPEC int GEP_HandleStartOfFrameMessages(char ** messages_in, char ** messages_out);

#ifdef __cplusplus
}
#endif

#endif