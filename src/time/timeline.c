#include "timeline.h"
#include <GLFW/glfw3.h>
timeline gen_timeline(void)
{
  timeline timeline_;
  timeline_.start_s = SDL_GetTicks();
  timeline_.last_update_s = timeline_.start_ms;
  return timeline_;
}
void free_timeline(timeline *timeline_)
{

}
timeline build_timeline(unsigned long start)
{
  timeline timeline_ = gen_timeline();
  timeline_.start_s = start;
  timeline_.last_update_s = start;
  return timeline_;
}
unsigned long elapsed_time(timeline timeline_)
{
  return timeline_.last_update_s - timeline_.start_s;
}
unsigned long update_timeline(timeline *timeline_)
{
  unsigned long diff = glfwGetTime() - timeline_->last_update_s;
  timeline_->last_update_s += diff;
  return diff;
}
