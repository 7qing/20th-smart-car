#ifndef _track_way_h_
#define _track_way_h_

#define search_count  200

void neighbor_track(uint8 flag);
void get_err();

extern uint8 l_border[image_h];//��������
extern uint8 r_border[image_h];//��������
extern uint8 center_line[image_h];//��������

#endif