#ifndef UTIL_FIFO_H
#define UTIL_FIFO_H



#include <stddef.h>
#include "Arduino.h"

#define RB_ATOMIC_START do { uint32_t _savedIS = xt_rsil(1) ;
#define RB_ATOMIC_END xt_wsr_ps(_savedIS) ;} while(0);


typedef struct FIFOBuffer
{
  unsigned char *begin;
  unsigned char *end;
  unsigned char * volatile head;
  unsigned char * volatile tail;
} FIFOBuffer;

inline bool fifo_isempty(const FIFOBuffer *f) {
  return f->head == f->tail;
}

inline bool fifo_isfull(const FIFOBuffer *f) {
  return ((f->head == f->begin) && (f->tail == f->end)) || (f->tail == f->head - 1);
}

inline void fifo_push(FIFOBuffer *f, unsigned char c) {
  *(f->tail) = c;
  
  if (f->tail == f->end) {
    f->tail = f->begin;
  } else {
    f->tail++;
  }
}

inline unsigned char fifo_pop(FIFOBuffer *f) {
  if(f->head == f->end) {
    f->head = f->begin;
    return *(f->end);
  } else {
    return *(f->head++);
  }
}

inline void fifo_flush(FIFOBuffer *f) {
  f->head = f->tail;
}

inline bool fifo_isempty_locked(const FIFOBuffer *f) {
  bool result;
  RB_ATOMIC_START
  {
    result = fifo_isempty(f);
  }
  RB_ATOMIC_END
  return result;
}

inline bool fifo_isfull_locked(const FIFOBuffer *f) {
  bool result;
  RB_ATOMIC_START
  {
    result = fifo_isfull(f);
   }
  RB_ATOMIC_END
  return result;
}

inline void fifo_push_locked(FIFOBuffer *f, unsigned char c) {
  RB_ATOMIC_START
  {
    fifo_push(f, c);
   }
  RB_ATOMIC_END
}

inline unsigned char fifo_pop_locked(FIFOBuffer *f) {
  unsigned char c;
  RB_ATOMIC_START
  {
    c = fifo_pop(f);
   }
  RB_ATOMIC_END
  return c;
}

inline void fifo_init(FIFOBuffer *f, unsigned char *buffer, size_t size) {
  f->head = f->tail = f->begin = buffer;
  f->end = buffer + size -1;
}

inline size_t fifo_len(FIFOBuffer *f) {
  return f->end - f->begin;
}

#endif