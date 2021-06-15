#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cgicc;

int main() {
  Cgicc formData;
  cout << "Content-type:text/html\r\n\r\n";
  cout << "<html>\n";
  cout << "<head>\n";
  cout << "<title>Drop Down Box Data to CGI</title>\n";
  cout << "</head>\n";
  cout << "<body>\n";
  form_iterator fi = formData.getElement("dropdown");
  if (!fi->isEmpty() && fi != (*formData).end()) {
    cout << "Value Selected: " << **fi << endl;
  }
  cout << "<br/>\n";
  cout << "</body>\n";
  cout << "</html>\n";
  return 0;
}
