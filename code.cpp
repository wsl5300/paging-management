# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <vector>
# include <queue>
# include <map>


using namespace std ;

class PR {

  struct DATA {
    int id = 0 ;
    vector <int> page ;
    bool replace = false ;
  };
  
  int buff ; // 容量
  vector <int> input ; // <6846513213132132>
  vector <DATA> write ;
  
  map <int,int> record ;
  
  int fault = 0 ;
  int replaces = 0 ;

  public :

    bool loading ( string &filename, int &command ) {
      input.clear() ;
      char t ;
      ifstream file ;
      cin >> filename ;
      filename = filename + ".txt" ;
      file.open( filename.c_str(), ios::in ) ;
      if ( !file.is_open()) {
        cout << "unfound file" << endl ;
        return false ;
      } // if

      file >> command ;
      file >> buff ;
      
      int i ;
      
      while ( file >> t ) {
        i = (int) t - '0' ;
        input.push_back(i) ;
        record[i] = 0 ;
      } // while

      file.close() ;

      
      return true ;
    } // load()

    int Choose ( DATA walk, bool fre ) {
      int maxp = 0 ; // 大的位置
      int pos = 0 ; // 小的位置
      
      for ( int i = 0 ; i < walk.page.size() ; i++ ) {
        if ( i == 0 ) pos = 0 ;
        else if ( !fre && record[walk.page[i]] < record[walk.page[pos]] ) pos = i ;
        else if ( fre && record[walk.page[i]] > record[walk.page[pos]] )  pos = i ;
      } // for
      
      record[walk.page[pos]] = 0 ;
      
      return pos ;
    } // Choose()

    void FIFO() {
      DATA temp ;
      
      for ( int i = 0 ; i < input.size() ; i++ ) {
        temp.id = input[i] ;
        temp.replace = false ;
        temp.page.clear() ;
        
        if ( i > 0 ) {
          for ( int j = 0 ; j < write[i-1].page.size() ; j++ ) { // 有無重複
            if ( temp.id == write[i-1].page[j] ) temp.replace = true ;
          } // for
          
          if ( !temp.replace ) { // 需要裝進去
            if ( write[i-1].page.size() < buff ) temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            else {
              temp.page.assign( write[i-1].page.begin()+1, write[i-1].page.end() ) ;
              replaces++ ;
            } // else
            
            temp.page.push_back( temp.id ) ;
            fault++ ;
          } // if
          else { // 不變
            temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
          } // else
          
        } // if
        else {
          fault++ ;
          temp.replace = false ;
          temp.page.push_back( temp.id ) ;
        } // else
        
        write.push_back( temp ) ;

        cout << temp.id << "\t" ;
        for ( auto a = temp.page.rbegin() ; a != temp.page.rend() ; a++ ) cout << *a ;
        cout << "\t" ;
        if ( !temp.replace ) cout << "F" << endl ;
        else cout << endl ;

      } // for
      

      cout << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

      
    } // FIFO()
    
    void LRU() {
      DATA temp ;

      for ( int i = 0 ; i < input.size() ; i++ ) {
        temp.id = input[i] ;
        temp.replace = false ;
        temp.page.clear() ;

        if ( i > 0 ) {
          for ( int j = 0 ; j < write[i-1].page.size() ; j++ ) { // 有無重複
            if ( temp.id == write[i-1].page[j]) temp.replace = true ;
          } // for

          if ( !temp.replace ) { // 需要裝進去
            if ( write[i-1].page.size() < buff ) temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            else {
              temp.page.assign( write[i-1].page.begin()+1, write[i-1].page.end() ) ;
              replaces++ ;
            } // else
            
            temp.page.push_back( temp.id ) ;
            fault++ ;
          } // if
          else { // 不變
            temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            for (int j = 0 ; j < temp.page.size() ; j++ )
              if ( temp.page[j] == temp.id ) temp.page.erase( temp.page.begin()+j ) ;
              
            temp.page.push_back(temp.id) ;
          } // else

        } // if
        else {
          fault++ ;
          temp.replace = false ;
          temp.page.push_back( temp.id ) ;
        } // else

        write.push_back( temp ) ;

        cout << temp.id << "\t" ;
        for ( auto a = temp.page.rbegin() ; a != temp.page.rend() ; a++ ) cout << *a ;
        cout << "\t" ;
        if ( !temp.replace ) cout << "F" << endl ;
        else cout << endl ;

      } // for


      cout << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

      
    } // LRU()
    
    void LFU() {
      DATA temp ;
      int pos = 0 ;

      for ( int i = 0 ; i < input.size() ; i++ ) {
        temp.id = input[i] ;
        temp.replace = false ;
        temp.page.clear() ;

        if ( i > 0 ) {
          for ( int j = 0 ; j < write[i-1].page.size() ; j++ ) { // 有無重複
            if ( temp.id == write[i-1].page[j] ) {
              temp.replace = true ;
              record[temp.id]++ ;
            } // if
          } // for

          if ( !temp.replace ) { // 需要裝進去
            if ( write[i-1].page.size() < buff ) temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            else {
              temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
              pos = Choose ( temp, false ) ;
              temp.page.erase( temp.page.begin()+pos ) ;
              replaces++ ;
            } // else
            temp.page.push_back( temp.id ) ;
            fault++ ;
          } // if
          else { // 不變
            temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
          } // else

        } // if
        else {
          fault++ ;
          temp.replace = false ;
          temp.page.push_back( temp.id ) ;
        } // else



        write.push_back( temp ) ;

        cout << temp.id << "\t" ;
        for ( int a = temp.page.size()-1 ; a >= 0 ; a-- ) cout << temp.page[a] ;
        cout << "\t" ;
        if ( !temp.replace ) cout << "F" << endl ;
        else cout << endl ;

      } // for


      cout << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

      
    } // LFU()
    
    void MFU() {
      DATA temp ;
      int pos = 0 ;

      for ( int i = 0 ; i < input.size() ; i++ ) {
        temp.id = input[i] ;
        temp.replace = false ;
        temp.page.clear() ;

        if ( i > 0 ) {
          for ( int j = 0 ; j < write[i-1].page.size() ; j++ ) { // 有無重複
            if ( temp.id == write[i-1].page[j] ) {
              temp.replace = true ;
              record[temp.id]++ ;
            } // if
          } // for

          if ( !temp.replace ) { // 需要裝進去
            if ( write[i-1].page.size() < buff ) temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            else {
              temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
              pos = Choose ( temp, true ) ;
              temp.page.erase( temp.page.begin()+pos ) ;
              replaces++ ;
            } // else
            
            temp.page.push_back( temp.id ) ;
            fault++ ;
          } // if
          else { // 不變
            temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
          } // else

        } // if
        else {
          fault++ ;
          temp.replace = false ;
          temp.page.push_back( temp.id ) ;
        } // else

        write.push_back( temp ) ;

        cout << temp.id << "\t" ;
        for ( int a = temp.page.size()-1 ; a >= 0 ; a-- ) cout << temp.page[a] ;
        cout << "\t" ;
        if ( !temp.replace ) cout << "F" << endl ;
        else cout << endl ;

      } // for


      cout << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

    } // MFU()
    
    void FR() {
      DATA temp ;
      int pos = 0 ;

      for ( int i = 0 ; i < input.size() ; i++ ) {
        temp.id = input[i] ;
        temp.replace = false ;
        temp.page.clear() ;

        if ( i > 0 ) {
          for ( int j = 0 ; j < write[i-1].page.size() ; j++ ) { // 有無重複
            if ( temp.id == write[i-1].page[j] ) {
              temp.replace = true ;
              record[temp.id]++ ;
            } // if
          } // for

          if ( !temp.replace ) { // 需要裝進去
            if ( write[i-1].page.size() < buff ) temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            else {
              temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
              pos = Choose ( temp, false ) ;
              temp.page.erase( temp.page.begin()+pos ) ;
              replaces++ ;
            } // else
            
            temp.page.push_back( temp.id ) ;
            fault++ ;
          } // if
          else { // 不變
            temp.page.assign( write[i-1].page.begin(), write[i-1].page.end() ) ;
            for (int j = 0 ; j < temp.page.size() ; j++ )
              if ( temp.page[j] == temp.id ) temp.page.erase( temp.page.begin()+j ) ;
            temp.page.push_back(temp.id) ;
          } // else

        } // if
        else {
          fault++ ;
          temp.page.push_back( temp.id ) ;
        } // else
        

        write.push_back( temp ) ;

        cout << temp.id << "\t" ;
        for ( int a = temp.page.size()-1 ; a >= 0 ; a-- ) cout << temp.page[a] ;
        cout << "\t" ;
        if ( !temp.replace ) cout << "F" << endl ;
        else cout << endl ;

      } // for


      cout << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

      
    } // FR()
    
    void Print( string filename, int command ) {
      filename =  "out_" + filename ;
	    ofstream out ( filename.c_str(), ios::app ) ;
	    if (out.is_open()) {
	      if ( command == 1 ) out << "--------------FIFO-----------------------\n" ;
	      else if ( command == 2 ) out << "--------------LRU-----------------------\n" ;
	      else if ( command == 3 ) out << "--------------Least Frequently Used Page Replacement-----------------------\n" ;
	      else if ( command == 4 ) out << "--------------Most Frequently Used Page Replacement -----------------------\n" ;
	      else if ( command == 5 ) out << "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ;

        for ( int j = 0 ; j < write.size() ; j++ ) {
	        out << write[j].id << "\t" ;
          for ( int a = write[j].page.size()-1 ; a >= 0 ; a-- ) out << write[j].page[a] ;
          out << "\t" ;
          if ( !write[j].replace ) out << "F" ;
          out << endl ;
        } // for
        out << "Page Fault = " << fault << "  Page Replaces = " << replaces << "  Page Frames = " << buff << endl ;

        out.close(); // 關閉檔案
      } // if
      else {
        cout << "Unable to open file\n";
      } // else
      
      write.clear() ;
      record.clear() ;
      fault = 0 ;
      replaces = 0 ;
      
      
    } // Print()


};



int main() {
  int command ;
  PR test ;
  string filename ;

  while( true ) {
		cout << "*** OS HomeWork 03 : Page Replacement    **" << endl ;
		cout << "* 0. QUIT                                 *" << endl ;
		cout << "* 1. First In First Out    (FIFO)         *" << endl ;
		cout << "* 2. Least Recently Used   (LRU)          *" << endl ;
		cout << "* 3. Least Frequently Used (LFU) + FIFO   *" << endl ;
		cout << "* 4. Most Frequently Used  (MFU) + FIFO   *" << endl ;
		cout << "* 5. LFU + LRU                            *" << endl ;
		cout << "* 6. All                                  *" << endl ;
		cout << "*************************************" << endl ;
		cout << "filename : " ;

    if ( !test.loading( filename, command ) ) {
      cout << "Please operate again.\n\n" ;
      continue ;
    } // if

    if ( command > 6 || command < 0 ) cout << "Command out of range !" << endl << endl ;
 	  else if ( command == 0 ) break ;
    else if ( command == 1 ) test.FIFO() ;
    else if ( command == 2 ) test.LRU() ;
	  else if ( command == 3 ) test.LFU() ;
	  else if ( command == 4 ) test.MFU() ;
	  else if ( command == 5 ) test.FR() ;
	  else if ( command == 6 ) {
	    test.FIFO() ;
	    test.Print( filename, 1 ) ;
	    test.LRU() ;
	    test.Print( filename, 2 ) ;
	    test.LFU() ;
	    test.Print( filename, 3 ) ;
	    test.MFU() ;
	    test.Print( filename, 4 ) ;
	    test.FR() ;
	    test.Print( filename, 5 ) ;
    } // else if

	  if ( command != 6 ) test.Print( filename, command ) ;
	  cout << endl << endl ;

	} // while
} // main()
