




// notifications display methods 

function showNotifications1(notifications){

    if(animationLock){
      setNotifications(notifications);

    $next = $('#eyes');
    $active = $('#notifications');

      transition( $active, $next);
    	 timerIdShowNotifications1 = setTimeout(function(){
       animationInit();
        console.log('showNotifications1() - end timer: '+timerIdShowNotifications1);
        //clearTimeout(timerId);
       
    	}, 5000);

     flipText(textOrientation);  
    console.log('showNotifications1() - begin timer: '+timerIdShowNotifications1);
}

}
function showNotifications2(notifications, period){
 
    animationLock = 0;  // we block the other animations 
    resetTimers();
    setNotifications(notifications);

    $next = $('#eyes');
    $active = $('#notifications');

     animationInit();
      $next.removeClass('active');
     $next.addClass('not-active');
     $active.removeClass('not-active');
     $active.addClass('active');
     
     timerIdShowNotifications2 = setTimeout(function(){
       animationInit();
         console.log('showNotifications2() - end timer: '+timerIdShowNotifications2);
      //  clearTimeout(timerId);
        animationLock = 1;   // unblock the other animations
       
      }, period*1000);
     flipText(textOrientation);  
     console.log('showNotifications2() - begin timer: '+timerIdShowNotifications2);

}

function showNotifications3(){
  hideEyes();
  hideCenterPanel();
  displayDate2();
  blinkFlag = false;
  var $panel = $('#principal-3');
  $panel.slideDown('slow');

  minutesTimer = setInterval(function(){
     displayDate2();
  }, 20000);


}

function showNotifications4(){


  displayDate2();
  blinkFlag = false;
  var $panel = $('#principal-3');

  minutesTimer = setInterval(function(){
     displayDate2();
  }, 20000);


}

function hideNotifications3(){
  blinkFlag = true;
  var $panel = $('#principal-3');
  $panel.slideUp('slow');
  showEyes();
  clearInterval(minutesTimer);
}
function setNotifications(notifications){

    // decoding the input 
    tabNotifications = notifications.split(":");
    newEmails = parseInt(tabNotifications[0]);
    wifiPct = parseInt(tabNotifications[1]);
    batteryPct = parseInt(tabNotifications[2]);
    
    
    notificationsInit();
    displayBatteryAndWifi(batteryPct, wifiPct );
    displayDate();
    $emails = $('#emails');
    $emails.html(newEmails);
    console.log('setNotifications');
}


function notificationsInit(){

     $battery =$('#battery-level');
     $wifi =$('#signal-level'); 
     $('#date').empty();
     $('#emails').empty();

     $battery.removeAttr('class');
     $battery.removeAttr('style');
     $wifi.removeAttr('class');
     $wifi.removeAttr('style');

     $battery.addClass('notfct-left-00');
     $wifi.addClass('notfct-right-00');
  console.log('notificationsInit');
}


// display the date on the Megamip eye notification panel 
function displayDate(){

    var $date =$('#date');
    $date.empty();
    // var d = new Date();
    var d = getMipDate();

      var month = d.getMonth()+1;
      var day = d.getDate();
      var hours = d.getHours();
      var minutes = d.getMinutes();
      var ampm  = "AM";
      if(hours > 12){
        ampm = "PM";
        hours -= 12;
      }
      var output1 = (hours<10?'0' : '')+hours+":"+(minutes<10?'0' : '')+minutes+ampm;
      var output2 = (day<10 ? '0' : '') + day+ '/'+(month<10 ? '0' : '') + month+ '/'+d.getFullYear();

      $date.html(output1+"<br/>"+output2);

      console.log('displayDate1');
  
}

// display the date on the full screen notification panel 
function displayDate2() {
    var $date = $('#pnl-date');
    var $time = $('#pnl-time');
    var $ampm = $('#pnl-am-pm');
 
    $date.empty();
    $time.empty();
    //var d = new Date();
     var d = getMipDate();

    var monthIndex = d.getMonth();
    var dayOfMonth = d.getDate();
    var dayIndex = d.getDay(); 
    var hours = d.getHours();
    var minutes = d.getMinutes();
    var ampm = "am";
    if (hours > 12) {
        ampm = "pm";
        hours -= 12;
    }

    var timeFormat =  hours + ":" + (minutes < 10 ? '0' : '') + minutes;
   // var dayFormat = (dayOfMonth < 10 ? '0' : '') + dayOfMonth + '/' + (month < 10 ? '0' : '') + month + '/' + d.getFullYear();
     var dayFormat = (dayOfMonth < 10 ? '0' : '') + dayOfMonth;
    $date.html('<ul><li>'+day[dayIndex]+'</li><li>'+month[monthIndex]+" "+'</li><li>'+
      dayFormat+'</li><li>'+d.getFullYear()+'</li></ul>');
    $time.html(timeFormat);
    $ampm.html("<br><br>"+ampm);

    console.log('displayDate2');

}



function displayBatteryAndWifi(batteryLevel, signalLevel){

    $battery = $("#battery-level");
    $signal = $("#signal-level");
     // setting the battery charge level
  

    if( batteryLevel > 80){        // battery level 100%
         $battery.removeAttr('class').addClass("notfct-left-05");

    }else if( 0 == batteryLevel){   // battery level 0%
         $battery.removeAttr('class').addClass("notfct-left-00");

    }else if( batteryLevel <= 20){   // battery level 20%
          $battery.removeAttr('class').addClass("notfct-left-01");

    }else if( batteryLevel <= 40){   // battery level 40%
          $battery.removeAttr('class').addClass("notfct-left-02");

    }else if( batteryLevel <= 60){   // battery level 60%
          $battery.removeAttr('class').addClass("notfct-left-03");

    }else{                           // battery level 80%
          $battery.removeAttr('class').addClass("notfct-left-04");
    }


 // setting the signal strenth level 

   if( signalLevel > 80){        // battery level 100%
         $signal.removeAttr('class').addClass("notfct-right-05");

    }else if( 0 == signalLevel){   // battery level 0%
         $signal.removeAttr('class').addClass("notfct-right-00");

    }else if( signalLevel <= 20){   // battery level 20%
          $signal.removeAttr('class').addClass("notfct-right-01");

    }else if( signalLevel <= 40){   // battery level 40%
          $signal.removeAttr('class').addClass("notfct-right-02");

    }else if( signalLevel <= 60){   // battery level 60%
          $signal.removeAttr('class').addClass("notfct-right-03");

    }else{                           // battery level 80%
          $signal.removeAttr('class').addClass("notfct-right-04");
    }


  console.log('displayBatteryAndWifi');
}




//------------------------------------------------
function blink(){
  if(!blinkFlag) return;

    if(animationLock){
      	$next = $('#eyes');
      	$active = $('#next');

        transition( $active, $next);
        timerIdBlink = setTimeout(function(){
         animationInit();
         console.log('blink end timer: '+timerIdBlink);
         // clearInterval(timerId);
      	}, 2200);
    }
    console.log('blink begin timer: '+timerIdBlink);
}


