var led = 0;

// Setting the pin to 0 turns the LED on
var led_off = 1;
var led_on = 0;

var digital_outs = [];

var leds = [LED1, LED2, LED3, LED4];

function connect_pins()
{
  print("Creating new DigitalOuts");
  digital_outs = [];
  for (var i = 0; i < 4; i++)
  {
    digital_outs.push(DigitalOut(leds[i], led_off));
    if (digital_outs[i].is_connected())
    {
      print("LED " + i + " is connected.");
    }
    else
    {
      print("LED " + i + " is not connected.");
    }
  }
}

connect_pins();

function blink()
{
  digital_outs[0].write(led_off);
  digital_outs[1].write(led_off);
  digital_outs[2].write(led_off);
  digital_outs[3].write(led_off);

  digital_outs[led].write(led_on);

  print("Finished with LED " + led);
  led = (led + 1) % 4;
}

// SW2 on FRDM-K64F and BUTTON2 on NRF52
var button;

if (typeof BUTTON2 !== 'undefined') {
    button = InterruptIn(BUTTON2);
} else {
    print("no button specified");
}
button.fall(function() {
  print("YOU PUSHED THE BUTTON!");
});

print("flash_leds.js has finished executing.");
