int has_extension(char *filename){
    for(;*filename != '\0';filename++){
        if (*filename == '.'){
            return 1;
        }
    }
    return 0;
}

void change_extension(char **filename,char *ext){
    int ln = strlen(*filename);
    int ext_ln = 0;
    int i;
    int nl;

    if (has_extension(*filename)){
        for(i = ln-1; i > 0; i--){
            if ((*filename)[i] == '.'){
                break;
            }
        }
        ext_ln = ln - i;
        if (ext_ln > strlen(ext) + 1){
            for(;*ext != '\0';ext++){
                (*filename)[i++] = *ext;
            }
            (*filename)[i] = '\0';
        }
        else{ /* we going to have to increase the filename */
            nl = ln + ((strlen(ext) + 1) - ext_ln);
            *filename = realloc(*filename,nl);
            if (*filename == NULL){
                fprintf(stderr, "Could not change the filename extension\n.");
                exit(1);
            }
            for(;*ext != '\0';ext++){
                (*filename)[i++] = *ext;
            }	
            (*filename)[i] = '\0';
        }
    }
    /* Does not have an extension */
    else {
        nl = ln + strlen(ext) + 1;
        *filename = realloc(*filename, ln + strlen(ext) + 1);
        if (*filename == NULL){
            fprintf(stderr, "Could not change the filename extension\n.");
            exit(1);
        }
        i = ln;
        for(;*ext != '\0';ext++){
            (*filename)[i++] = *ext;
        }
        (*filename)[i] = '\0';
    }
}	    
