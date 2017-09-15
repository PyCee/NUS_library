#include "NUS_frame.h"
#include "NUS_animation.h"
#include "../NUS_log.h"

NUS_frame nus_frame_build(NUS_animation *p_animation)
{
  NUS_frame frame = {
    .p_animation = p_animation,
    .pose = nus_skeleton_pose_build(p_animation->p_skeleton),
    .progress = 0.0f,
    .repeat = NUS_FALSE
  };
  nus_frame_update(&frame, 0.0f);
  return frame;
}
void nus_frame_free(NUS_frame *p_frame)
{
  nus_skeleton_pose_free(&p_frame->pose);
}
void nus_frame_update(NUS_frame *p_frame, float delta_s)
{
  NUS_keyframe lerped_keyframe;
  float t;
  int i;
  p_frame->progress += (delta_s / p_frame->p_animation->duration);
  if(p_frame->progress > 1.0f){
    if(p_frame->repeat){
      p_frame->progress -= 1.0f;
    } else {
      // Max progress to 1.0f
      p_frame->progress = 1.0f;
    }
  }
  for(i = 0; i < p_frame->p_animation->frame_count - 1; ++i){
    // For each frame

    if(p_frame->progress >= p_frame->p_animation->times[i] &&
       p_frame->progress <= p_frame->p_animation->times[i + 1]){
      // If progress is between the selected 2 frames, or equal to the first
      // i represents the first of the selected frames
      //   (the second selected frame is indexed i + 1)
      break;
    }
  }
  
  if(i == p_frame->p_animation->frame_count - 1){
    --i;
    NUS_LOG_ERROR("frame i not right\n");
  }
  
  // t is the progress between the 2 frames, t = (0, 1)
  t = (p_frame->progress - p_frame->p_animation->times[i]) /
    (p_frame->p_animation->times[i + 1] - p_frame->p_animation->times[i]);

  if(t < 0.0f || t > 1.0f){
    NUS_LOG_ERROR("t in frame update is out of bounds. t = %f\n", t);
  }
  
  lerped_keyframe = nus_keyframe_build(p_frame->p_animation->p_skeleton->joint_count);
  nus_keyframe_lerp(&lerped_keyframe, p_frame->p_animation->keyframes[i],
		    p_frame->p_animation->keyframes[i+1],
		    p_frame->p_animation->p_skeleton->joint_count, t);
  
  nus_skeleton_pose_update(&p_frame->pose, lerped_keyframe);
  nus_keyframe_free(&lerped_keyframe);
}
void nus_frame_set_repeat(NUS_frame *p_frame, NUS_bool repeat)
{
  p_frame->repeat = repeat;
}
NUS_bool nus_frame_is_finished(NUS_frame frame)
{
  return ((1.0 - frame.progress) < 0.00001) ? NUS_TRUE : NUS_FALSE;
}
