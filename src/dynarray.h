//---------------------------------------------------------------------------

#ifndef DynArrayH
#define DynArrayH
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#define TDYNARRAY_INCREMENT 1000
#define TDYNARRAY_DEFAULT_VALUE 0
//#define TDYNARRAY_MAX 100000
template <class T> class TDynArray{

   private:

     T *data;
     T *temp;
     int data_len;
     int data_full;

     int _increment;

   public:

     //throw -1 cannot resize object (malooc or realloc failed)
     //throw -2 error reading object (access overflow)

     TDynArray(void){
         data=NULL;
         data_len=0;
         data_full=0;
         temp=NULL;
         _increment=TDYNARRAY_INCREMENT;
     }

     ~TDynArray(void){
         if (data!=NULL) delete[] data;
         data=NULL;
     }

     void SetIncrement(int inc){
       if (inc>0) _increment=inc;
     }

     const int Length(void) const { return data_full; }

     T* Data(void){ return data; }

     void SetLength(int new_len){
        int i;
        if (data_len<new_len){
           if (data_full==0 && data==NULL) {
             while (data_len<new_len) data_len+=_increment;
             data=new T[data_len];
             data_full=0;
           }
           else{
             while (data_len<new_len) data_len+=_increment;
             temp=data;
             data=new T[data_len];
             for (i=0;i<data_full;i++) data[i]=temp[i];
             delete[] temp;
             temp=NULL;
           }
        }
        else if (new_len==0){
           if (data!=NULL) delete[] data;
           data=NULL;
           data_full=0;
           data_len=0;
        }

        if (new_len>0 && data==NULL){
          data_full=0;
          data_len=0;
          throw -1;
        }
        else data_full=new_len;
     }

     void empty(void){
        SetLength(0);
     }

     int insert(int offset, T *buf, int buf_len){
        int old_len=data_full;
        int i;
        if (offset>=data_full) offset=data_full;
        SetLength(data_full+buf_len);
        for (i=old_len-offset-1;i>=0;i--) data[i+offset+buf_len]=data[i+offset];
        for (i=0;i<buf_len;i++) data[i+offset]=buf[i];
        return 0;
     }

     int insert(int offset, T buf){
        int old_len=data_full;
        int i;
        if (offset>=data_full) offset=data_full;
        SetLength(data_full+1);
        for (i=old_len-offset-1;i>=0;i--) data[i+offset+1]=data[i+offset];
        data[offset]=buf;
        return 0;
     }

     int add(T *buf, int buf_len){
        return insert(data_full,buf,buf_len);
     }

     int add(T buf){
        return insert(data_full,buf);
     }

     int remove(int offset, int buf_len){
        int old_len=data_full;
        int i;
        if (offset>=data_full) return 0;
        if (offset+buf_len>=data_full) buf_len=data_full-offset;
        if (old_len-offset-buf_len>0)
        for (i=0;i<old_len-offset-buf_len;i++) data[i+offset]=data[i+offset+buf_len];
        SetLength(data_full-buf_len);
        return 0;
     }

     T *get(const int i){
       if (i<0 || i>=data_full) throw -2;
       return &data[i];
     }

     T &operator [] (int i){
       if (i<0 || i>=data_full) throw -2;
       return data[i];
     };

     const T &operator [] (int i) const {
       if (i<0 || i>=data_full) throw -2;
       return data[i];
     };
};



#endif
