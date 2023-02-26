const String ris = R"VOG(<html><head><title>EspRemote</title></head><body>
"<div align='center'>
"<h1>EspRemote</h1>
"<form method='post'>
  SSID:<br>
  <input type='text' name='ssid' value='SSS'><br><br>
  Password:<br>
  <input type='password' name='password' value='PPP'><br><br>
  
  <input type='checkbox' name='client' CCC> Is client<br>
  
  Connection timeout (client only):<br>
  <input type='number' name='timeout' value='TTT' min='1' max='60'><br><br>

  <input type='checkbox' name='dhcp' DDD> Use DHCP*<br>
  *(ip, subnet and gateway are ignored if checked)<br>
  IP:<br>
  <input type='text' name='ip' value='XXX'><br>
  Subnet:<br>
  <input type='text' name='subnet' value='YYY'><br><br>
  Gateway:<br>
  <input type='text' name='gateway' value='ZZZ'><br><br>
  
  Serial baudrate: (upload is always 115200)<br>
  <input type='text' name='baudrate' value='BBB'><br><br>
  
  
  <input type='submit' value='Submit'>
</form>
</div>
</body></html>)VOG";
