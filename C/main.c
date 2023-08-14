#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON/cJSON.h>


void jsonToCSV(const char *jsonFileName, const char *csvFileName){

    FILE *jsonFile = fopen(jsonFileName, "r");
    if (jsonFile == NULL) {
        perror("Error opening JSON file");
        return;
    }

    fseek(jsonFile, 0, SEEK_END);
    long fileSize = ftell(jsonFile);
    fseek(jsonFile, 0, SEEK_SET);

    char *jsonBuffer = (char *)malloc(fileSize + 1);
    if (jsonBuffer == NULL) {
        fclose(jsonFile);
        perror("Memory allocation error");
        return;
    }

    fread(jsonBuffer, 1, fileSize, jsonFile);
    fclose(jsonFile);

    jsonBuffer[fileSize] = '\0';

    cJSON *json = cJSON_Parse(jsonBuffer);
    free(jsonBuffer);

    if (json == NULL) {
        const char *error = cJSON_GetErrorPtr();
        if (error != NULL) {
            fprintf(stderr, "JSON Error: %s \n", error);
        }
    }

    FILE *csvFile = fopen(csvFileName, "w");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        cJSON_Delete(json);
        return;
    }

    cJSON *firstObject = cJSON_GetArrayItem(json, 0);
    cJSON *currentItem = firstObject->child;
    while (currentItem){
        fprintf(csvFile, "%s,",currentItem->string);
        currentItem = currentItem->next;
    }

    fprintf(csvFile, "\n");

    // Write CSV rows
    cJSON *currentObject = json->child;
    while (currentObject){
        currentItem = currentObject->child;
        while (currentItem){
            if(currentItem->type == cJSON_String){
                fprintf(csvFile, "%s,", currentItem->valuestring);
            }else if(currentItem->type == cJSON_Number){
                fprintf(csvFile, "%d,", currentItem->valueint);
            }else if(currentItem->type == cJSON_True){
                fprintf(csvFile, "true,");
            }else if(currentItem->type == cJSON_False){
                fprintf(csvFile, "false,");
            }else {
                fprintf(csvFile, ",");
            }

            currentItem = currentItem->next;
        }
        fprintf(csvFile, "\n");
        currentObject = currentObject->next;
    }

    fclose(csvFile);
    cJSON_Delete(json);

}

int main() {
    const char *jsonFileName = "../sample.json";
    const char *csvFileName = "output.csv";

    jsonToCSV(jsonFileName, csvFileName);

    printf("conversion complete. \n");

    return 0;

}
