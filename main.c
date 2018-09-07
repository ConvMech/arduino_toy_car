#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define N     128
#define BUF_SIZE      32

static int spl = 0, spr = 0;

char buf[BUF_SIZE];

int start_flag = 0;
int count_flag = 0;

char    buf_out[N];
int     value;
int     count = 0;
char    buf1[4];
char    buf2[4];
short   i, j;
short   k[3];
int     sl = 0, sr = 0;


//For left wheel
int     sl_fix = 0;
double  sl_target = 0;

//For right wheel
int     sr_fix = 0;
double  sr_target = 0;


void loop(char *buffer)
{
    memset(buf_out, '\0',  N);
    memset(buf1,    '\0',  4);
    memset(buf2,    '\0',  4);

    if(1){ //
        count = 0;
        j     = 0;
        value = 0;

        k[0] = k[1] = k[2] = 0;
        count = sizeof(buffer)/sizeof(buffer[0]);
        start_flag = 1;
    }
    count_flag = 0;
    if(start_flag){
        if(buffer[count-3]=='e' && buffer[count - 2] == 'n' && buffer[count-1] == 'c'){
            printf("I got enc!\n");
        }
        else if(buffer[count-3]=='r' && buffer[count - 2] == 'e' && buffer[count-1] == 's'){
            printf("I got res!\n");
        }
        else{
            for(i = 0; i < count; i++){
                if(buffer[i] == 'p' || buffer[i] == 'm'){
                    k[j] = i;
                    j++;
                }
            }
            if(j == 3){
                strncpy(buf1, buffer + k[0] + 1, k[1] - k[0] - 1);
                strncpy(buf2, buffer + 1 + k[1] , k[2] - k[1] - 1);
                if(abs(sl_fix - atol(buf1))){
                    sl_fix = atol(buf1);
                    //sl = sl_fix;
                }
                if(abs(sr_fix - atol(buf2))){
                    sr_fix = atol(buf2);
                    //sr = sr_fix;
                }
                if(sl < 256 && sr < 256){
                    if(buffer[k[0]] == 'm'){
                        //analogWrite(L_PWM, sl);
                        //digitalWrite(L_DIR, HIGH);
                        printf("set L_DIR HIGH\n");
                    }
                    else{
                        //analogWrite(L_PWM, sl);
                        //digitalWrite(L_DIR, LOW);
                        printf("set L_DIR LOW\n");
                    }
                    if(buffer[k[1]] == 'm'){
                        //analogWrite(R_PWM, sr);
                        //digitalWrite(R_DIR, LOW);
                        printf("set R_DIR LOW\n");
                    }
                    else{
                        //analogWrite(R_PWM, sr);
                        //digitalWrite(R_DIR, HIGH);
                        printf("set R_DIR HIGH\n");
                    }
                }
            }

            sl_target = sl_fix / 255.0;
            sr_target = sr_fix / 255.0;

            printf("sl_target is %f, sr_target is %f \n",sl_target,sr_target);

        }
    }
}

int
act_conv_velocity_to_pulse(float velocity /* [m/s] */)
{
    int pulse = 0;
    pulse = 255 * velocity;
    if(pulse > 0)
        pulse = pulse + 20;
    if(pulse < 0)
        pulse = pulse - 20;
    if(pulse >= 255)
        pulse = 255;
    if(pulse <= -255)
        pulse = -255;
    return pulse;
}

short act_set_velocities(float left_velocity, float right_velocity)
{

    memset(buf, 0, BUF_SIZE);
    spl = -act_conv_velocity_to_pulse(right_velocity);
    spr = -act_conv_velocity_to_pulse(left_velocity);

    if(spl < 0){
        if(spr < 0)
            sprintf(buf, "m%dm%dp:", -spl, -spr);
        else
            sprintf(buf, "m%dp%dp:", -spl, spr);
    }
    else{
        if(spr < 0)
            sprintf(buf, "p%dm%dp:", spl, -spr);
        else
            sprintf(buf, "p%dp%dp:", spl, spr);
    }
    printf("sl=%d sr=%d\n",spl,spr);
    printf("%s\n", buf);
    return 0;
}

int main() {
    act_set_velocities(0.2,0.3);
    loop(buf);
    printf("Hello, World!\n");
    return 0;
}