#define ALPHA 1
#define BETA 3


void split(char *str, char *output1, char *output2, char *output3){
    char * pch;
    uint8_t k = 0;
    // Serial.println("Splitting string  into tokens:");
    pch = strtok (str,"-");
    while (pch != NULL)
    {
        if (k == 0)
            strcpy(output1, pch);
        if (k == 1)
            strcpy(output2, pch);
        if (k == 2)
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
    uint8_t dev_rssi;
    uint8_t dev_transmissions;
};

struct BleDevice devices_record[5];

void readStruct(){
    for (i = 0; i < dev_index; i++){
        Serial.println(devices_record[i].dev_mac);
    }
}

int getScore(uint8_t rssi, uint8_t transmissions){
    return ALPHA*rssi + BETA*transmissions; 
}

uint8_t selectDevice(){
    uint8_t selectedDevice = 9;
    int min_score = 900;
    for(i = 0; i<dev_index; i++){
        if(getScore(devices_record[i].dev_rssi, devices_record[i].dev_transmissions) < min_score){
            if (devices_record[i].dev_transmissions < 50){
                selectedDevice = i;
                min_score = getScore(devices_record[i].dev_rssi, devices_record[i].dev_transmissions);
            }
        }
    }
    Serial.println(selectedDevice);
    return selectedDevice;
}

void storeInStruct(char *curr_mac, char* curr_id, uint8_t curr_rssi, uint8_t curr_transmission){    
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

bool getName(char *line, char *current_name){
    if(strstr(line, "OK+NAME")){
        getString(line, current_name, 10);
        // Serial.println(current_name);
        return true; 
    } else {
        return false;
    }
}

void getString(char *line, char* return_string, uint8_t str_len){
    char *start = strchr(line, ':');
    uint8_t index = (uint8_t)(start - line+1);
    strncpy(return_string, &line[index], str_len);
    return_string[str_len-1] = '\0';
}