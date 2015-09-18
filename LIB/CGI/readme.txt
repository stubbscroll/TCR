cgi example!

very basic cgi info:

1. cgi-script must output content type, which is "Content-type: text/html"
   for html
2. parameters to url (the stuff that follows ?) can be read from the
   environment variable QUERY_STRING
3. put file in cgi-bin/ or give the executable .cgi extension
4. make sure the executable actually runs on the web server. if possible,
   compile it on the server
