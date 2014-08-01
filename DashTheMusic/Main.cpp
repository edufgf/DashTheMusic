#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

typedef struct _ball{
    ALLEGRO_BITMAP *bmp;
    float rad;
    float px[310], py[310];
    float offy[45];
    float vel,acc;
    int p1,p2;
}Ball;

typedef struct _note{
    ALLEGRO_BITMAP *bmp;
    ALLEGRO_TIMER* timer;
    float px,py,alt,larg,vel;
    float altOrig,largOrig;
    float delay;
    float time,acc;
}Note;

typedef struct _sprite{
    ALLEGRO_BITMAP *bmp;
    float px,py,alt,larg,vel;
    float altOrig,largOrig;
    float acc;
}Sprite;


const float FPS = 60;
const float M_TIME = 500;
const int BTIME = 30;
const int LOCK_X = 1;
float FIXED_X;
int ACCELERATE = 0;
int start_ind,end_ind;
int notes_qtd;
int tiles_qtd;
int speed_ind=0;
int speed_end_ind[10];
float speed_mult;
float time_dist;

ALLEGRO_DISPLAY *display;
ALLEGRO_DISPLAY_MODE disp_data;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_TIMER *speed_up;
ALLEGRO_TIMER *speed_stop;
ALLEGRO_TIMER *mouse_timer;
ALLEGRO_SAMPLE *song1;
ALLEGRO_BITMAP *title;
Sprite tile[3];
Note note[500];
Ball ball;

void init_ball(){
    ball.rad = (float)disp_data.width/64;

    for (int i=0;i<300;i++){
       ball.px[i]=disp_data.width/2;
       ball.py[i]=disp_data.height/2;
    }
    for (int i=0;i<21;i++)
       ball.offy[i]=i-10;
    for (int i=21;i<41;i++)
       ball.offy[i]=ball.offy[20-(i-20)];

    ball.p1=0;
    ball.p2=0;
    ball.vel=0.2*(2*ball.rad);
    FIXED_X = 0.35*disp_data.width;
}

void init_tiles(){
    for (int i=0;i<3;i++){
        tile[i].bmp = al_load_bitmap("tile1.png");

        tile[i].altOrig = al_get_bitmap_height(tile[i].bmp);
        tile[i].alt = 0.15*disp_data.height;
        tile[i].largOrig = al_get_bitmap_width(tile[i].bmp);
        tile[i].larg = tile[i].largOrig;

        tile[i].px = i*tile[i].largOrig;
        tile[i].py = disp_data.height-0.15*disp_data.height;
        tile[i].vel = 4;
    }
    tiles_qtd=3;
}

void init_title(){
    title = al_load_bitmap("title.png");
    al_draw_bitmap(title, 0, 0, 0);

    al_flip_display();
}

void build_note(float x, float y, float larg, float alt, float vel, string s, Note* note){
    note->bmp = al_load_bitmap(s.c_str());
    note->largOrig = al_get_bitmap_width(note->bmp);
    note->altOrig = al_get_bitmap_height(note->bmp);
    note->px = x;
    note->py = y;
    note->vel = vel;
    note->larg = larg;
    note->alt = alt;
}

void copy_note(Note* note, Note* source, string s){
    note->bmp = al_load_bitmap(s.c_str());
    note->largOrig = al_get_bitmap_width(note->bmp);
    note->altOrig = al_get_bitmap_height(note->bmp);
    note->px = source->px;
    note->py = source->py;
    note->vel = source->vel;
    note->larg = source->larg;
    note->alt = source->alt;
}

void build_note2(Note* top, Note* down, float x, float alt1, float dist, float larg, float vel, string s){
    top->bmp = al_load_bitmap(s.c_str());
    top->largOrig = al_get_bitmap_width(top->bmp);
    top->altOrig = al_get_bitmap_height(top->bmp);
    down->bmp = al_load_bitmap(s.c_str());
    down->largOrig = al_get_bitmap_width(down->bmp);
    down->altOrig = al_get_bitmap_height(down->bmp);

    top->px = x;
    down->px = x;

    top->py = 0;
    down->py = alt1+dist;

    top->vel = vel;
    down->vel = vel;
    top->larg = larg;
    down->larg = larg;

    top->alt = alt1;
    down->alt = disp_data.height-(down->py);
}

void init_notes(){
    int i;

    note[0].time=2.216;
    note[1].time=2.532;
    note[2].time=2.900;
    note[3].time=3.306;
    note[4].time=3.621;
    //
    note[5].time=5.970;
    note[6].time=6.622;
    note[7].time=7.315;
    //
    note[8].time=9.750;
    note[9].time=10.100;
    note[10].time=10.486;
    note[11].time=10.904;
    note[12].time=11.347;
    //
    note[13].time=13.491;
    note[14].time=13.491;
    note[15].time=14.200;
    note[16].time=14.200;
    note[17].time=14.911;
    note[18].time=14.911;
    //
    note[19].time=17.352;
    note[20].time=17.352;
    note[21].time=17.663;
    note[22].time=17.663;
    note[23].time=18.004;
    note[24].time=18.004;
    note[25].time=18.371;
    note[26].time=18.371;
    note[27].time=18.696;
    note[28].time=18.696;
    //
    note[29].time=21.058;
    note[30].time=21.058;
    note[31].time=21.742;
    note[32].time=21.742;
    note[33].time=22.460;
    note[34].time=22.460;
    //
    note[35].time=25.045;
    note[36].time=25.278;
    note[37].time=25.278;
    note[38].time=25.570;
    note[39].time=25.933;
    note[40].time=25.933;
    note[41].time=26.240;
    note[42].time=26.240;
    //
    note[43].time=28.700;
    note[44].time=29.352;
    note[45].time=29.352;
    note[46].time=29.958;
    //
    note[47].time=32.485;
    note[48].time=32.961;
    note[49].time=32.961;
    note[50].time=33.405;
    note[51].time=33.832;
    note[52].time=33.832;
    note[53].time=34.357;
    note[54].time=34.357;
    //
    note[55].time=34.974;
    note[56].time=34.974;
    note[57].time=35.223;
    note[58].time=35.223;
    note[59].time=35.412;
    note[60].time=35.412;
    note[61].time=35.612;
    note[62].time=35.612;
    note[63].time=35.883;
    note[64].time=35.883;
    //
    note[65].time=36.362;
    note[66].time=36.751;
    note[67].time=36.751;
    note[68].time=37.197;
    note[69].time=37.667;
    note[70].time=37.667;
    note[71].time=38.110;
    note[72].time=38.110;
    //
    note[73].time=38.561;
    note[74].time=38.561;
    note[75].time=38.810;
    note[76].time=38.810;
    //
    note[77].time=40.087;
    note[78].time=40.087;
    note[79].time=40.563;
    note[80].time=40.563;
    note[81].time=41.007;
    note[82].time=41.007;
    note[83].time=41.434;
    note[84].time=41.434;
    note[85].time=41.759;
    note[86].time=41.759;
    //
    note[87].time=41.964;
    note[88].time=41.964;
    note[89].time=42.310;
    note[90].time=42.310;
    note[91].time=42.781;
    note[92].time=42.781;
    //
    note[93].time=43.793;
    note[94].time=44.093;
    note[95].time=44.426;
    note[96].time=44.726;
    note[97].time=45.162;
    note[98].time=45.462;
    //
    note[99].time=45.827;
    note[100].time=45.827;
    note[101].time=46.033;
    note[102].time=46.033;
    note[103].time=46.341;
    note[104].time=46.341;
    // dif - 476 - 444 - 427 - 525 ()
    note[105].time=47.548;
    note[106].time=48.024;
    note[107].time=48.024;
    note[108].time=48.468;
    note[109].time=48.895;
    note[110].time=48.895;
    note[111].time=49.420;
    note[112].time=49.420;
    // folow up 249-189-200-271
    note[113].time=50.166;
    note[114].time=50.166;
    note[115].time=50.415;
    note[116].time=50.415;
    note[117].time=50.604;
    note[118].time=50.604;
    note[119].time=50.804;
    note[120].time=50.804;
    note[121].time=51.075;
    note[122].time=51.075;
    // folow up dif2  389 - 446 - 470 - 443
    note[123].time=51.297;
    note[124].time=51.686;
    note[125].time=51.686;
    note[126].time=52.132;
    note[127].time=52.602;
    note[128].time=52.602;
    note[129].time=53.045;
    note[130].time=53.045;
    //dif 3 249
    note[131].time=53.575;
    note[132].time=53.575;
    note[133].time=53.824;
    note[134].time=53.824;

    // dif - 476 - 444 - 427 - 525 (325)
    note[135].time=55.074;
    note[136].time=55.550;
    note[137].time=55.550;
    note[138].time=55.994;
    note[139].time=56.421;
    note[140].time=56.421;
    note[141].time=56.746;
    note[142].time=56.746;
    // folow up WAT 205(come) - 346 -
    note[143].time=57.031;
    note[144].time=57.031;
    note[145].time=57.454;
    note[146].time=57.454;
    note[147].time=57.887;
    note[148].time=57.887;

    // folow up dif2  389 - 446 - 470 - 443
    note[149].time=58.877;
    note[150].time=59.266;
    note[151].time=59.266;
    note[152].time=59.712;
    note[153].time=60.182;
    note[154].time=60.182;
    note[155].time=60.625;
    note[156].time=60.625;
    //dif 3 249
    note[157].time=61.220;
    note[158].time=61.220;
    note[159].time=61.469;
    note[160].time=61.469;

    notes_qtd=161;
    //speed_end_ind[0]=47;
    //build_note x, y, larg, alt, vel, bmp
    build_note(disp_data.width,0,50,(float)disp_data.height/5,10,"wood1.png",&note[13]);
    copy_note(&note[17],&note[13],"wood1.png");
    build_note(disp_data.width,(float)disp_data.height/5+5*ball.rad,50,disp_data.height-((float)disp_data.height/5+5*ball.rad)
                           ,10,"wood1.png",&note[14]);
    copy_note(&note[18],&note[14],"wood1.png");
    build_note(disp_data.width,0,50,4*(float)disp_data.height/5,10,"wood1.png",&note[15]);
    build_note(disp_data.width,4*(float)disp_data.height/5+5*ball.rad,50,disp_data.height-(4*(float)disp_data.height/5+5*ball.rad),10,"wood1.png",&note[16]);

    build_note2(&note[19],&note[20],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,30,10,"wood1.png");
    build_note2(&note[21],&note[22],disp_data.width,(float)disp_data.height/2-1*ball.rad,5*ball.rad,30,10,"wood1.png");
    build_note2(&note[23],&note[24],disp_data.width,((float)disp_data.height/2)-6*ball.rad,5*ball.rad,30,10,"wood1.png");
    build_note2(&note[25],&note[26],disp_data.width,((float)disp_data.height/4),4*ball.rad,30,10,"wood1.png");
    build_note2(&note[27],&note[28],disp_data.width,((float)disp_data.height)/4+5*ball.rad,5*ball.rad,30,10,"wood1.png");

    build_note2(&note[29],&note[30],disp_data.width,(float)disp_data.height/8+1*ball.rad,5*ball.rad,50,10,"wood1.png");
    build_note2(&note[31],&note[32],disp_data.width,4*(float)disp_data.height/6+1*ball.rad,5*ball.rad,50,10,"wood1.png");
    build_note2(&note[33],&note[34],disp_data.width,(float)disp_data.height/2-1*ball.rad,5*ball.rad,50,10,"wood1.png");

    build_note(disp_data.width,0,30,(float)disp_data.height-5*ball.rad,10,"wood1.png",&note[35]);
    build_note2(&note[36],&note[37],disp_data.width,(float)disp_data.height-10*ball.rad,5*ball.rad,30,10,"wood1.png");
    build_note(disp_data.width,0,30,(float)disp_data.height-5*ball.rad,10,"wood1.png",&note[38]);
    build_note2(&note[39],&note[40],disp_data.width,2*(float)disp_data.height/3-1*ball.rad,4*ball.rad,30,10,"wood1.png");
    build_note2(&note[41],&note[42],disp_data.width,(float)disp_data.height/3+2*ball.rad,6*ball.rad,30,10,"wood1.png");

    build_note(disp_data.width,5*ball.rad,50,(float)disp_data.height-5*ball.rad,13,"wood1.png",&note[43]);
    build_note2(&note[44],&note[45],disp_data.width,10*ball.rad,4*ball.rad,50,13,"wood1.png");
    build_note(disp_data.width,5*ball.rad,50,(float)disp_data.height-5*ball.rad,13,"wood1.png",&note[46]);

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[47]);
    build_note2(&note[48],&note[49],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[50]);
    build_note2(&note[51],&note[52],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[53],&note[54],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,400,30,"wood1.png");

    build_note2(&note[55],&note[56],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[57],&note[58],disp_data.width,(float)disp_data.height/2-4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[59],&note[60],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[61],&note[62],disp_data.width,(float)disp_data.height/2-4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[63],&note[64],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[65]);
    build_note2(&note[66],&note[67],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[68]);
    build_note2(&note[69],&note[70],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[71],&note[72],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,400,30,"wood1.png");

    build_note2(&note[73],&note[74],disp_data.width,(float)disp_data.height/4+1*ball.rad/3,5*ball.rad,30,30,"wood1.png");
    build_note2(&note[75],&note[76],disp_data.width,(float)disp_data.height/2-2*ball.rad/3,5*ball.rad,30,30,"wood1.png");

    build_note2(&note[77],&note[78],disp_data.width,(float)disp_data.height/5,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[79],&note[80],disp_data.width,(float)disp_data.height/2+2*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[81],&note[82],disp_data.width,(float)disp_data.height/5,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[83],&note[84],disp_data.width,(float)disp_data.height/2+2*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[85],&note[86],disp_data.width,2*(float)disp_data.height/5-2*ball.rad,5*ball.rad,200,30,"wood1.png");

    build_note2(&note[87],&note[88],disp_data.width,(float)disp_data.height/2-ball.rad/2,5*ball.rad,100,30,"wood1.png");
    build_note2(&note[89],&note[90],disp_data.width,(float)disp_data.height/3-ball.rad/2,5*ball.rad,100,30,"wood1.png");
    build_note2(&note[91],&note[92],disp_data.width,2*(float)disp_data.height/3+ball.rad/2,5*ball.rad,100,30,"wood1.png");

    build_note(disp_data.width,(float)disp_data.height/5+4*ball.rad,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[93]);
    build_note(disp_data.width,0,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[94]);
    build_note(disp_data.width,(float)disp_data.height/5+4*ball.rad,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[95]);
    build_note(disp_data.width,0,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[96]);
    build_note(disp_data.width,(float)disp_data.height/5+4*ball.rad,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[97]);
    build_note(disp_data.width,0,100,4*(float)disp_data.height/5-4*ball.rad,30,"wood1.png",&note[98]);

    build_note2(&note[99],&note[100],disp_data.width,3*(float)disp_data.height/4-ball.rad/2,4*ball.rad,120,30,"wood1.png");
    build_note2(&note[101],&note[102],disp_data.width,2*(float)disp_data.height/4-ball.rad/2,4*ball.rad,120,30,"wood1.png");
    build_note2(&note[103],&note[104],disp_data.width,(float)disp_data.height/4+2*ball.rad/2,4*ball.rad,120,30,"wood1.png");

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[105]);
    build_note2(&note[106],&note[107],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[108]);
    build_note2(&note[109],&note[110],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[111],&note[112],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,400,30,"wood1.png");

    build_note2(&note[113],&note[114],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[115],&note[116],disp_data.width,(float)disp_data.height/2-4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[117],&note[118],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[119],&note[120],disp_data.width,(float)disp_data.height/2-4*ball.rad,6*ball.rad,120,30,"wood1.png");
    build_note2(&note[121],&note[122],disp_data.width,(float)disp_data.height/2+4*ball.rad,6*ball.rad,120,30,"wood1.png");

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[123]);
    build_note2(&note[124],&note[125],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[126]);
    build_note2(&note[127],&note[128],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[129],&note[130],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,400,30,"wood1.png");

    build_note2(&note[131],&note[132],disp_data.width,(float)disp_data.height/4+1*ball.rad/3,5*ball.rad,30,30,"wood1.png");
    build_note2(&note[133],&note[134],disp_data.width,(float)disp_data.height/4-10*ball.rad/3,5*ball.rad,30,30,"wood1.png");

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[135]);
    build_note2(&note[136],&note[137],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[138]);
    build_note2(&note[139],&note[140],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[141],&note[142],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,200,30,"wood1.png");

    build_note2(&note[143],&note[144],disp_data.width,(float)disp_data.height/4-ball.rad/2,4*ball.rad,50,30,"wood1.png");
    build_note2(&note[145],&note[146],disp_data.width,3*(float)disp_data.height/4-ball.rad/2,5*ball.rad,50,30,"wood1.png");
    build_note2(&note[147],&note[148],disp_data.width,(float)disp_data.height/4-ball.rad/2,4*ball.rad,50,30,"wood1.png");

    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[149]);
    build_note2(&note[150],&note[151],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note(disp_data.width,0,400,(float)disp_data.height-5*ball.rad,30,"wood1.png",&note[152]);
    build_note2(&note[153],&note[154],disp_data.width,3*(float)disp_data.height/4-6*ball.rad,5*ball.rad,400,30,"wood1.png");
    build_note2(&note[155],&note[156],disp_data.width,(float)disp_data.height/2-4*ball.rad,5*ball.rad,400,30,"wood1.png");

    build_note2(&note[157],&note[158],disp_data.width,(float)disp_data.height/4+1*ball.rad/3,5*ball.rad,30,30,"wood1.png");
    build_note2(&note[159],&note[160],disp_data.width,(float)disp_data.height/4-10*ball.rad/3,5*ball.rad,30,30,"wood1.png");

    for (i=0;i<13;i++){
       if (i>=5 && i<=7) continue;
       note[i].bmp = al_load_bitmap("wood1.png");
       note[i].largOrig = al_get_bitmap_width(note[i].bmp);
       note[i].altOrig = al_get_bitmap_height(note[i].bmp);
       note[i].px = disp_data.width;
       note[i].vel = 10;
       note[i].larg = 30;
       note[i].alt=disp_data.height/2 + ball.rad;

       if (i%2!=0){
          if (i<8)
            note[i].py=0;
          else
            note[i].py=disp_data.height/2 - ball.rad;
       }else{
          if (i<8)
            note[i].py=disp_data.height/2 - ball.rad;
          else
            note[i].py=0;
       }
    }

    for (i=5;i<8;i++){
        note[i].bmp = al_load_bitmap("wood1.png");
        note[i].px = disp_data.width;
        note[i].py = (float)disp_data.height/3;
        note[i].alt = 2*(float)disp_data.height/3;
        note[i].larg = 50;
        note[i].vel = 10;
        note[i].largOrig = al_get_bitmap_width(note[i].bmp);
        note[i].altOrig = al_get_bitmap_height(note[i].bmp);
    }
    note[6].py = 0;

    for (i=0;i<notes_qtd;i++){
        note[i].delay=((disp_data.width-FIXED_X)/note[i].vel)*(1.0/FPS);
    }

    for (int i=0;i<notes_qtd;i++){
       note[i].timer = al_create_timer(note[i].time-note[i].delay);
       al_register_event_source(event_queue, al_get_timer_event_source(note[i].timer));
    }


    start_ind=0;
    end_ind=0;
}

void init_song(){
    song1 = al_load_sample( "splinter.flac" );
}

void init_timers(){
    timer = al_create_timer(1.0 / FPS);
    mouse_timer = al_create_timer(1.0 / M_TIME);

    speed_up = al_create_timer(25);
    speed_stop = al_create_timer(33);
}

void init_display(){
    al_init_image_addon();
    al_init_primitives_addon();

    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(disp_data.width, disp_data.height);
}

void init_event_queue(){
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(mouse_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(speed_up));
    al_register_event_source(event_queue, al_get_timer_event_source(speed_stop));
}

void start_up(){
    al_start_timer(timer);
    al_start_timer(mouse_timer);

    for (int i=0;i<notes_qtd;i++)
      al_start_timer(note[i].timer);

    al_start_timer(speed_up);
    al_start_timer(speed_stop);

    al_play_sample(song1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

int init(){
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   if(!al_install_mouse()) {
      fprintf(stderr, "failed to initialize the mouse!\n");
      return -1;
   }

   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   }

   if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
   }

   if(!al_init_acodec_addon()) {
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
   }

   if (!al_reserve_samples(1)){
      fprintf(stderr, "failed to reserve samples!\n");
      return -1;
   }

   init_timers();

   init_display();

   init_event_queue();

   init_ball();

   init_tiles();

   init_title();

   init_notes();

   init_song();

   ALLEGRO_EVENT ev;
   while(1){
      al_wait_for_event(event_queue, &ev);
      if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
           if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
               break;
      }
   }

   start_up();
}

void exit(){
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
}

void draw_ball(){
    float x,y,off,r,px0,py0,mpx,mpy,dist,d2;
    int i,p,p2;

    p  = ball.p1-1,
    p2 = ball.p2;

    r = ball.rad;
    dist = r/3;
    d2 = ball.vel;

    if (p<0) p+=300;

    mpx=ball.px[p];
    mpy=ball.py[p];

    for(i=0;i<10;i++){
       x = ball.px[p];
       y = ball.py[p];

       if (!LOCK_X){
           if (i!=0){
              if (fabs(x-mpx)<=(i*d2+dist) && x<=mpx && fabs(y-mpy)<=dist){
                 x = px0-d2+(i*1);
              }
           }
           px0=x;
           py0=y;
           ball.px[p]=px0;
           ball.py[p]=py0;
       }

       p2-=i;
       if (p2<0)
          p2+=41;
       off = ball.offy[p2];

       if (!LOCK_X)
         al_draw_filled_circle(x, y-off, r, al_map_rgb(198,226,255));
       else{
         if (i==0){
           al_draw_filled_circle(FIXED_X, y-off, r, al_map_rgb(198,226,255));
           px0 = FIXED_X;
         }else{
           al_draw_filled_circle(px0-d2+(i*1), y-off, r, al_map_rgb(198,226,255));
           px0 = px0-d2+(i*1);
         }
       }
       r = r*0.8;
       p = (p-BTIME);
       if (p<0) p+=300;
    }
    ball.p2 = (ball.p2+1)%41;
    if (ACCELERATE){
        ball.vel+=ball.acc;
    }
}

void ball_shift(float x,float y){
     int po = ball.p1;
     if (x-ball.rad>0 && x+ball.rad<disp_data.width && y-ball.rad>0 && y+ball.rad<disp_data.height){
        ball.px[po]=x;
        ball.py[po]=y;
     }
     if (x-ball.rad>0 && x+ball.rad<disp_data.width){
        ball.px[po]=x;
     }else if (x-ball.rad>0){
        ball.px[po]=disp_data.width-ball.rad;
     }else ball.px[po]=ball.rad;

     if (y-ball.rad>0 && y+ball.rad<disp_data.height){
        ball.py[po]=y;
     }else if (y-ball.rad>0){
        ball.py[po]=disp_data.height-ball.rad;
     }else ball.py[po]=ball.rad;

     ball.p1 = (ball.p1+1)%300;
}

void draw_scaled(Sprite* s){
   al_draw_scaled_bitmap(s->bmp, 0, 0, s->largOrig, s->altOrig,
                         s->px, s->py, s->larg, s->alt, 0);
   s->px-=s->vel;
   if (s->px<-s->larg) s->px += s->larg*3;
   if (ACCELERATE){
       s->vel+=s->acc;
   }
}

void draw_tiles(){
   for (int i=0;i<3;i++)
      draw_scaled(&tile[i]);
}

void draw_obst(){
   int i;
   for (i=start_ind;i<end_ind;i++){
       al_draw_scaled_bitmap(note[i].bmp, 0, 0, note[i].largOrig, note[i].altOrig,
       note[i].px, note[i].py, note[i].larg, note[i].alt, 0);

       //if (ACCELERATE){
      //    note[i].vel+=note[i].acc;
      // }
       note[i].px-=note[i].vel;
       if (note[i].px<-note[i].larg){
          start_ind=i+1;
       }
   }
  // if (ACCELERATE)
   //  for (i=end_ind;i<speed_end_ind[speed_ind];i++)
     //     note[i].vel+=note[i].acc;

}

int main(int argc, char **argv){

   int ret = init();
   if (ret==-1) return -1;

   bool redraw = false;
   ALLEGRO_EVENT ev;
   ALLEGRO_MOUSE_STATE mstate;

   while(1){
      al_wait_for_event(event_queue, &ev);
      if(ev.timer.source == timer) {
         redraw = true;
      }else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
              ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
         ball_shift(ev.mouse.x,ev.mouse.y);
      }else if (ev.timer.source == mouse_timer){
         al_get_mouse_state(&mstate);
         ball_shift(mstate.x,mstate.y);
      }else if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
               (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) )  {
            break;
      }

      if (ev.type == ALLEGRO_EVENT_TIMER){
         if (ev.timer.source == speed_up){
             ACCELERATE=1;
             speed_mult=4;
             time_dist=8;
             for (int j=0;j<tiles_qtd;j++)
                tile[j].acc = (float)(tile[j].vel*(speed_mult-1))/(time_dist*FPS);

             for (int j=start_ind;j<notes_qtd;j++)
                note[j].acc = (float)(note[j].vel*(speed_mult-1))/(time_dist*FPS);

             ball.acc = (float)(ball.vel*(speed_mult-3))/(time_dist*FPS);
             al_destroy_timer(speed_up);
         }else if (ev.timer.source == speed_stop){
             ACCELERATE=0;
             speed_ind++;
             al_destroy_timer(speed_up);
         }else{
             for (int i=start_ind;i<notes_qtd;i++){
                if (ev.timer.source == note[i].timer){
                   end_ind=i+1;
                   al_destroy_timer(note[i].timer);
                   break;
                }
             }
         }
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         al_clear_to_color(al_map_rgb(0,0,0));

         draw_tiles();
         draw_ball();
         draw_obst();

         al_flip_display();
      }
   }

   exit();

   return 0;
}
