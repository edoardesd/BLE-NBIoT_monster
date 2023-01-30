#define ALPHA 1
#define BETA 3


void split(char *str, char *output1, char *output2, char *output3){
    char * pch;
    int i = 0;
    // Serial.println("Splitting string  into tokens:");
    pch = strtok (str,"-");
    while (pch != NULL)
    {
        if (i == 0)
            strcpy(output1, pch);
        if (i == 1)
            strcpy(output2, pch);
        if (i == 2)
            strcpy(output3, pch);
        pch = strtok (NULL, "-");
        i++;
    }
}


const char *known_mac[] = {"A06C65CF8E11", "94A9A83B7B35", "A06C65CF7F9C"};
int dev_index = 0;
bool read_next = false;
char id[3];
char rssi[4];
char transmissions[4];
char designatedMAC[13];

struct BleDevice{
    char dev_mac[13];
    char dev_id[3];
    int dev_rssi;
    int dev_transmissions;
};

struct BleDevice devices_record[3];

void readStruct(){
    for (i = 0; i < dev_index; i++){
        Serial.println(devices_record[i].dev_rssi);
    }
}

int getScore(int rssi, int transmissions){
    return ALPHA*rssi + BETA*transmissions; 
}

int selectDevice(){
    int selectedDevice = 9;
    int min_score = 900;
    for(i = 0; i<dev_index; i++){
        if(getScore(devices_record[i].dev_rssi, devices_record[i].dev_transmissions) < min_score){
            if (devices_record[i].dev_transmissions < 50){
                selectedDevice = i;
            }
        }
    }

    return selectedDevice;
}

void storeInStruct(char *curr_mac, char* curr_id, int curr_rssi, int curr_transmission){    
    // Serial.println(curr_mac);
    strcpy(devices_record[dev_index].dev_mac, curr_mac);
    // Serial.println(devices_record[dev_index].dev_mac);

    strcpy(devices_record[dev_index].dev_id, curr_id);
    devices_record[dev_index].dev_rssi = curr_rssi;
    devices_record[dev_index].dev_transmissions = curr_transmission;

    dev_index++;
    read_next = false;
}

bool check_known_mac(char *current_mac){
    for (i = 0; i < 3; i++){
        if (strstr(known_mac[i], current_mac)){
            // Serial.println("known");
            return true;
        }
    }
    return false;
}

bool getMac(char *line, char *current_mac){
   bool read = false;
   if(strstr(line, "OK+DIS0:") || strstr(line, "OK+DIS1:")){
        getString(line, current_mac, 13);
        read = check_known_mac(current_mac);
        return read; 
    } else {
        return read;
    }
}

int getName(char *line, char *current_name){
    if(strstr(line, "OK+NAME")){
        getString(line, current_name, 10);
        // Serial.println(current_name);
        return 1; 
    } else {
        return 0;
    }
}

void getString(char *line, char* return_string, uint8_t str_len){
    char *start = strchr(line, ':');
    uint8_t index = (uint8_t)(start - line+1);
    strncpy(return_string, &line[index], str_len);
    return_string[str_len-1] = '\0';
}