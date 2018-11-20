void Diagnostic(){  
  for (int i=0; i<13; i++){
    Serial.print("Light Errors: ");
    if (light[i]==0){
      Serial.print(i);
      Serial.print(" ");      
    }
  }
  Serial.println(" ");
}

