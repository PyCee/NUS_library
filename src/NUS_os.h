#ifndef _NUS_OS_H_
#define _NUS_OS_H_

#if defined(_WIN32)
#define NUS_OS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__unix__)
#define NUS_OS_UNIX
#define VK_USE_PLATFORM_XCB_KHR
#endif

#endif /* _NUS_OS_H_ */
