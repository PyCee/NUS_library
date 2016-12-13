#include "timeline.h"
#include <SDL2/SDL.h>

timeline gen_timeline(void)
{
  timeline timeline_;
  timeline_.start_ms = SDL_GetTicks();
  timeline_.last_update_ms = timeline_.start_ms;
  return timeline_;
}
void free_timeline(timeline *timeline_)
{

}
timeline build_timeline(unsigned long start)
{
  timeline timeline_ = gen_timeline();
  timeline_.start_ms = start;
  timeline_.last_update_ms = start;
  return timeline_;
}
unsigned long elapsed_time(timeline timeline_)
{
  return timeline_.last_update_ms - timeline_.start_ms;
}
unsigned long update_timeline(timeline *timeline_)
{
  unsigned long diff = SDL_GetTicks() - timeline_->last_update_ms;
  timeline_->last_update_ms += diff;
  return diff;
}
