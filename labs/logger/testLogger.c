int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main(){
    infof("INFO: Important information.");
    warnf("WARNING: Maybe there's something not going as expected." );
    errorf("ERROR: Wrong usage.");
    panicf("PANIC: Stop it!");
    return 0;
}