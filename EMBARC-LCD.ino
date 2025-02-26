 #include <LiquidCrystal.h>
 #include <Wire.h>

int contador = 0;

 LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

 void setup()
 {
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Permitido!");
 Wire.begin(8); // Inicia como escravo com endereço 8
 lcd.setCursor(0, 1);
 lcd.print("Contador: 0");
 Wire.onReceive(receberEvento); // Registra a função para receber dados
 }

 void loop()
 {
 }


void receberEvento(int quantos) {
    while (Wire.available()) {
        char comando = Wire.read();

        if (comando == 'B') {
			    lcd.clear();
 			    lcd.setCursor(0, 0);
 			    lcd.print("Bloqueado!");
        } else if (comando == 'P') {
      lcd.clear();
 			lcd.setCursor(0, 0);
 			lcd.print("Permitido!");
      lcd.setCursor(0, 1);
      lcd.print("Contador: 0");
      contador = 0;
        } else if (comando == 'I') {
          lcd.clear();
 			    lcd.setCursor(0, 0);
 			    lcd.print("Permitido!");
          lcd.setCursor(0, 1);
          lcd.print("Contador: ");
          contador++;
          lcd.print(contador);
        }
    }
}