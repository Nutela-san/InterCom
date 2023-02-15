#include "InterCom.h"

SimpleComand::SimpleComand(){
    //nada
}

void SimpleComand::begin(unsigned long baud_rate){
    Serial.begin(baud_rate);
    port_comunication = &Serial;
    port_comunication->println("Terminal inicializadad...");
}

void SimpleComand::addComand(char cmd_name[],float *dato2modify){
    uint8_t comand_lenght = sizeof(cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_comand_available && comand_lenght<= max_comand_lenght){
        strcpy(cmd_list[cmd_list_count].comand_name,cmd_name);
        cmd_list[cmd_list_count].data2change = dato2modify;
    }
    cmd_list_count++;
}

void SimpleComand::addComand(char cmd_name[],void (*subRutine)(void)){
    uint8_t comand_lenght = sizeof(cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_comand_available && comand_lenght<= max_comand_lenght){
        strcpy(cmd_list2[cmd_list_count].comand_name,cmd_name);
        cmd_list2[cmd_list_count].subRutine = subRutine;
    }
    cmd_list_count++;
}

void SimpleComand::list(void){
    char msj[50];
    sprintf(msj,"-- Lista de Comandos --");
    port_comunication->println(msj);
    for(uint8_t i=0;i<max_comand_available;i++){
        if(strlen(cmd_list[i].comand_name) != 0) {
            sprintf(msj,"%d.- '%s'",i+1, cmd_list[i].comand_name);
            port_comunication->println(msj);
        }else if(strlen(cmd_list2[i].comand_name) != 0){
            sprintf(msj,"%d.- '%s'",i+1, cmd_list2[i].comand_name);
            port_comunication->println(msj);
        }
    }
}

void SimpleComand::lisent(void){
    while(port_comunication->available()){
        char ch = port_comunication->read();
        bool endline = check_endline(ch);
        if(ch != '\r' && !endline) cmd_readed[coun_char_read] = ch;
        if(en_echo && ch != '\r') port_comunication->write(ch);
        if(endline){
            check_cmd();
            coun_char_read = 0;
            for(uint8_t i=0;i<max_comand_lenght;i++){ //vaciando string
                cmd_readed[i]= 0;
            }
        }
        else if (coun_char_read>= max_comand_lenght){
            coun_char_read = 0;
            for(uint8_t i=0;i<max_comand_lenght;i++){ //vaciando string
                cmd_readed[i]= 0;
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
        if(cmpCMDs(cmd_readed,cmd_list[i].comand_name)){
            num_cmd = i;
            i = max_comand_available;
            exist = 1;
        }
        else if (cmpCMDs(cmd_readed,cmd_list2[i].comand_name)){
            num_cmd = i;
            i = max_comand_available;
            exist = 2;
        }
        else{
            exist = 0;
        }
    }
        
    char mensj[50];
    switch(exist){
        case 1:{
            i=strlen(cmd_list[num_cmd].comand_name);
            for(uint8_t j=i;j<max_comand_lenght;j++){
                cmd_readed[j-i]= cmd_readed[j];
            }
            for(uint8_t j=max_comand_lenght-i;j<max_comand_lenght;j++){
                cmd_readed[j]= 0;
            }
            if(strlen(cmd_readed)!=0) *cmd_list[num_cmd].data2change = atof(cmd_readed);
            sprintf(mensj,"%s = %f",cmd_list[num_cmd].comand_name,*cmd_list[num_cmd].data2change);
            break;
        }
        case 2:{
            cmd_list2[num_cmd].subRutine();
            for(i=0;i<50;i++){
                mensj[i] = 0;
            }
            break;
        }
        case 0: {
            sprintf(mensj,"Error -%s-",cmd_readed);
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