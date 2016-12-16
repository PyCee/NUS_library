#ifndef _TIMELINE_H_
#define _TIMELINE_H_

typedef struct timeline{
  unsigned long start_ms, last_update_ms;
} timeline;
timeline gen_timeline(void);
void free_timeline(timeline *);
timeline build_timeline(unsigned long);
unsigned long elapsed_time(timeline);
unsigned long update_timeline(timeline *);

#endif /* _TIMELINE_H_ */
