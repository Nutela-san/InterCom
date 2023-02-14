#include "InterCom.h"

SimpleComand::SimpleComand(){
    //nada
}

void SimpleComand::begin(unsigned long baud_rate){
    Serial.begin(baud_rate);
    port_comunication = &Serial;
    port_comunication->println("Terminal inicializadad...");
}

void SimpleComand::addComand(char cmd_name[],double *dato2modify){
    uint8_t comand_lenght = sizeof(cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_comand_available && comand_lenght<= max_comand_lenght){
        strcpy(comand_list[cmd_list_count].comand_name,cmd_name);
        comand_list[cmd_list_count].data2change = dato2modify;
        comand_list[cmd_list_count].num_list = cmd_list_count;
    }
    cmd_list_count++;
}

void SimpleComand::addComand(char cmd_name[],void (*subRutine)(void)){
    uint8_t comand_lenght = sizeof(cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_comand_available && comand_lenght<= max_comand_lenght){
        strcpy(comand_list2[cmd_list_count].comand_name,cmd_name);
        comand_list2[cmd_list_count].num_list = cmd_list_count;
        comand_list2[cmd_list_count].subRutine = subRutine;
    }
    cmd_list_count++;
}

void SimpleComand::list(void){
    char msj[50];
    sprintf(msj,"-- Lista de Comandos --");
    port_comunication->println(msj);
    for(uint8_t i=0;i<max_comand_available;i++){
        if(strlen(comand_list[i].comand_name) != 0) {
            sprintf(msj,"%d.- '%s'",i+1, comand_list[i].comand_name);
            port_comunication->println(msj);
        }else if(strlen(comand_list2[i].comand_name) != 0){
            sprintf(msj,"%d.- '%s'",i+1, comand_list2[i].comand_name);
            port_comunication->println(msj);
        }
    }
}

void SimpleComand::lisent(void){
    while(port_comunication->available()){
        char ch = port_comunication->read();
        comand_readed[coun_char_read] = ch;
        if(en_echo && !check_endline(ch)) port_comunication->write(ch);
        if(check_endline(ch)){
            check_cmd();
            coun_char_read = 0;
            for(uint8_t i=0;i<max_comand_lenght;i++){ //vaciando string
                comand_readed[i]= (char)0;
            }
        }
        else if (coun_char_read>= max_comand_lenght){
            coun_char_read = 0;
            for(uint8_t i=0;i<max_comand_lenght;i++){ //vaciando string
                comand_readed[i]= (char)0;
            }
        }
        else{
            coun_char_read ++;
        }
        
    }
}

void SimpleComand::check_cmd(void){
    uint8_t exist = 0,i;
    uint8_t num_cmd;
    for(i=0;i<max_comand_available;i++){
        if(cmpCMDs(comand_readed,comand_list[i].comand_name)){
            num_cmd = i;
            i = max_comand_available;
            exist = 1;
        }
        else if (cmpCMDs(comand_readed,comand_list2[i].comand_name)){
            num_cmd = i;
            i = max_comand_available;
            exist = 2;
        }
    }
        
    char mensj[50];
    switch(exist){
        case 1:{
            i=0;
            while(comand_list[num_cmd].comand_name[i]!=0)i++;
            for(uint8_t j=0;j<max_comand_lenght;j++){
                if(comand_readed[j+i]!=0){
                    comand_readed[j]= comand_readed[j+i];
                }
                else{
                    comand_readed[j]= 0;
                }
            }
            if(strlen(comand_readed)!=0){
                sprintf(mensj,"%s = %s",comand_list[num_cmd].comand_name,comand_readed);
                *comand_list[num_cmd].data2change = strtod(comand_readed,0);
            }
            else{
                sprintf(mensj,"%s = %f",comand_list[num_cmd].comand_name,*comand_list[num_cmd].data2change);
            }
            break;
        }
        case 2:{
            comand_list2[num_cmd].subRutine();
            break;
        }
        default: {
            sprintf(mensj,"Error -%s-",comand_readed);
            break;
        }
    }
    if(en_echo) port_comunication->println(mensj);
}

bool cmpCMDs(char cmd_in[], char cmd_list[]){
    uint8_t i;
    bool igual = false;
    for(i=0;i<max_comand_lenght;i++){
        if(cmd_list[i]!= 0){
            if(cmd_in[i] == cmd_list[i]){
                igual = true;
            }
            else{
                igual = false;
                i = max_comand_lenght;
            }
        }
    }
    return igual;
}

bool SimpleComand::check_endline(char c){
    if(c == end_line) return true;
    return false;
}

void SimpleComand::enable_echo(bool en){ 
    en_echo = en;
}