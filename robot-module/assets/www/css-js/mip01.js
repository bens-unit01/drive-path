




// var global

var index = 0; 
var selectedItem = null;
var timerIdShowNotifications1 = null;
var timerIdShowNotifications2 = null;
var timerIdBlink = null;
var player = null;
var animationLock = 1;
var mode = 'picture';
var textOrientation = 1; // 0 = normal, 1 = mirrored 
var month = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
var day = ['sun', 'mon', 'tue', 'wed', 'thu', 'fri', 'sat'];
var minutesTimer;
// time management variables 
var offset = {"montreal":-4, "tokyo":9, "san_francisco":-7};
var mLocation = 'montreal';
var mOffset = offset.montreal;
var blinkFlag = true;


//--

function flipText(pos){

  textOrientation = pos;
  var $text = $('.text-field');
  var $notifications_pnl3 = $('#principal-3');
  var $notifications_pnl2 = $('#principal-2');
  if(pos == 1){
       $text.each(function(index){
             $(this).addClass('mirror');
        
       });

       $notifications_pnl2.addClass('mirror');
       $notifications_pnl3.addClass('mirror');
 
  }else{

     $text.each(function(index){
             $(this).removeClass('mirror');
        
       });
      $notifications_pnl2.removeClass('mirror');
      $notifications_pnl3.removeClass('mirror');
  }
  

}
//----------animations 

function sequence01(){    // transition - shrink 


// fade-out fade-in effect ---------------------
 var $active = $('#eyes');
 var $next = $("#next");
transition($next, $active);


// shrink effect ----------------------------------
       var timerId1 = setInterval(
    function(){
      
    $active.removeAttr('style'); // resetting the style 
    $next.removeAttr('style');
     shrink();
      clearInterval(timerId1);

    }, 2000);


// count down animation  ----------------------------------

 var timerId2 = setInterval(
    function(){
      
    $active.removeAttr('style'); // resetting the style 
    $next.removeAttr('style');
     animationInit();
     setFrameTo(2);
     loop(2,4);
     clearInterval(timerId2);

    }, 3000);

//call to the "speak now"   ( onScreen ...) method ---------
    var timerId3 = setInterval(
    function(){
      
     // call through the javascript interface to show up the "speak now" screen
     try{  
     megaMipJSInterface.onSpeak();
      }catch(error){
        console.log("bloc catch err: "+error);
      }
     clearInterval(timerId3);
     animationInit();  // reset the eyes to the initial state 

    }, 4500);
   
  // console.log('sequence01 '+timerId1+' '+timerId2+' '+timerId3);
   
    }




function setFrameTo(n){

  $("#eyes").removeAttr('class');
  $("#eyes").removeAttr('style');
  $("#next").removeAttr('class');
  $("#next").removeAttr('style');
  $("#notifications").removeAttr('class');
  $("#notifications").removeAttr('style');
  $("#shrink").removeAttr('class');
  $("#shrink").removeAttr('style');

  $("#eyes").addClass('active eyes-state-0'+n);
  $("#next").addClass('not-active eyes-state-01');
  $("#notifications").addClass('not-active eyes-state-08');
  $("#shrink").addClass('not-active');
 // console.log('setFrameTo');

 }
    function shrink(){

 // $.setDisabledByDefault(true);
   var $active = $( "#eyes" );
   var $next  = $("#shrink");

   
   $active.removeClass('eyes-state-01').addClass('eyes-state-09');
   $next.removeClass('not-active').addClass('active');

   $next.transition({ scale:1.2, delay: 200});
   $next.transition({ scale:0.5});
   console.log('shrink');
   }

   function transition( $active, $next){

     $next.fadeTo(2000, 0.25);
     $active.fadeTo(2000, 1);
     $next.fadeOut();
   //  console.log('transition ');

    }

    function animationInit(){
        $("#eyes").removeAttr('class');
        $("#eyes").removeAttr('style');
        $("#next").removeAttr('class');
        $("#next").removeAttr('style');
        $("#notifications").removeAttr('class');
        $("#notifications").removeAttr('style');
        $("#shrink").removeAttr('class');
        $("#shrink").removeAttr('style');
        
        $("#eyes").addClass('active eyes-state-00');
        $("#next").addClass('not-active eyes-state-10');
        $("#notifications").addClass('not-active eyes-state-08');
        $("#shrink").addClass('not-active');

       console.log('animationInit');
    }


function loop(firstFrame,lastFrame){
  var $active = $( "#eyes" );
  var i=firstFrame;var j=0;
  var timerId = setInterval(function(){
    j=i; j++;
   if( i != lastFrame+1){
      $( "#eyes" ).removeClass("eyes-state-0"+i);
      $( "#eyes" ).addClass("eyes-state-0"+j);
      i++;

   }else{
       
         clearInterval(timerId);
    }

    },300);
 // console.log('loop()');
}
   
//------------------
function showMic(){

var $eyes = $( "#eyes" );
$eyes.removeClass("eyes-state-00").addClass("eyes-state-01");

 var $active = $eyes;
 var i=1;var j=0;
// starting the mic animation 
 var timerId = setInterval(function(){
  j=i; j++;
    if( i != 7){
 $eyes.removeClass("eyes-state-0"+i);
 $eyes.addClass("eyes-state-0"+j);
i++;

 }else{
    
    
    $eyes.removeClass("eyes-state-0"+i);
    $eyes.addClass("eyes-state-01");
    i=1; 
  // clearInterval(timerId);

  }

},300);
//console.log('showMic() ...');
}


function hideMic(){

  if(timerId != null){
   clearInterval(timerId);  // we stop the mic animation 
    var currentClass = $( "#eyes" ).attr('class');
   $( "#eyes" ).removeClass(currentClass).addClass('eyes-state-00');
   timerId = null;
  // console.log('hideMic');
 }
}
function next(){

    if(baseRef != null){
         baseRef.buttonNext.trigger('touchend.owlControls');
    }
   
}

function prev(){

    if(baseRef != null){
         baseRef.buttonPrev.trigger('touchend.owlControls');
    }
   
}

function show(mMode){
    
    mode = mMode;
   // selectedItem = $("[data-wow-index ='"+ index+"'] a").trigger("click");
   selectedItem = $("[data-wow-"+baseRef.currentItem+"] ");
   var url = selectedItem.attr("data-wow-"+baseRef.currentItem);
   console.log('show --- url: '+url);
   if(mMode == 'video' || mMode == 'local_video' )
   {

     var mode =(mMode == 'video')? 0 : 1;  // 0 for youtube videos, 1 for local videos 
   try{  
     
      megaMipJSInterface.onLaunchVideo(url, mode);
     
      }catch(error){
        console.log("show() -- bloc catch err: "+error);
      }
    }


  //  console.log('show');
}

function go(){
    index = 0;
   var keyword = $("#txt_input").val();
   $("#results").empty();
   googleSearch( keyword);
 //   console.log('go');
}

function clearScreen(){
    
    index = 0;
    $("#results").empty();
 // console.log('clearScreen');
}
function back(){    
  if(mode = 'picture'){
   $.fancybox.close() 
   console.log('fancybox.close');
   }   
}


function showEyes(){
 $("#eyes").slideDown("slow");
 animationInit();
 animationLock = 1;
 console.log("showEyes ");
 //clearTimeout(timerId);
resetTimers();
// console.log('showEyes');

}
function hideEyes(){
  $("#eyes").slideUp("slow"); 
 // console.log('hideEyes');  
}
function showCenterPanel(){
     $("#principal-2").slideDown("slow");  
  //   console.log('showCenterPanel'); 
}
function hideCenterPanel() {
     $("#principal-2").slideUp("slow");  
      $("#principal-3").slideUp("slow");    
  //   console.log('hideCenterPanel');
}

function log(displayString){
    hideEyes();showCenterPanel();
    $('#output').empty();
    $('#output').append(displayString);

    var timerShow = setInterval(
    function(){
      
      hideCenterPanel();
      showEyes();
      clearInterval(timerShow);

    }, 2000);
 //   console.log('log');
}

function resetTimers(){
 clearTimeout(timerIdBlink);
 clearTimeout(timerIdShowNotifications1);
 clearTimeout(timerIdShowNotifications2);
 clearTimeout(minutesTimer);
}


//--------------------------------------------


function pictureSearch(keyword){
    
   
  
  var apiURL = 'http://ajax.googleapis.com/ajax/services/search/'+"images"+
                        '?v=1.0&callback=?';
 var params = {
                q : keyword,
                rsz :8,
                start : 0
              };
 $.ajax({
        url:apiURL,
        type:"GET",
        dataType : "JSON",
        data: params
        , success: function(json){

          var content = "";
          var newWidth = 0;
         animationInit();
         clearScreen();
          hideEyes();     
          showCenterPanel();
          var $results =  $('#results');
         
          if(baseRef != null){
           var items = baseRef.itemsAmount;

           for(var i=0 ; i < items; i++){
              $results.data('owlCarousel').removeItem();
            }

          }else{
             $results.owlCarousel({
             stopOnHover : true,
             navigation:true,
             paginationSpeed : 200,
             goToFirstSpeed : 200,
             singleItem : true,
             transitionStyle:"fade"

              // navigation : true, // Show next and prev buttons
              // slideSpeed : 300,
              // paginationSpeed : 400,
              // singleItem:true
          });

           }
          
        //  $results.trigger('destroy.owl');
          // $results.empty();
          // $results.removeAttr('style');
          //  $results.removeClass('owl-theme');


         // content = "<div id='owl-demo' class='owl-carousel'></div>"
         // $results.append(content);

          for(var i = 0; i < 6; i++){

             content = "<div><img src='"+json.responseData.results[i].url + "'/></div>";
             //$results.append(content);
              $results.data('owlCarousel').addItem(content);

           } 
          
       
          
         

         //"<div><img src='"+json.responseData.results[i].tbUrl + "'/></div>";

                        //  "<a href='"+json.responseData.results[i].url+"'>"+
                         //-- select the first item
                          // var s =  $('.pic').first()
                          //         .removeClass("not-selected")
                          //         .addClass("selected");
                          
                          //--FancyBox --
                              //  $("a").fancybox({
                              //     'overlayShow' : false,
                              //     'transitionIn'  : 'elastic',
                              //     'transitionOut' : 'elastic',
                              //     'closeClick':true, 
                              //     'closeBtn':false,
                              //     padding:0
                              // });

                       
     
   
    }, 
    error: function(xhr, textStatus, errorThrown){
      window.setTimeout($.ajax(this, 1000));
    }
  });
 
}
// video search functions  -------------------------------


function videoSearch(keyword){
    
   
  
 var apiURL = 'https://gdata.youtube.com/feeds/api/videos?q='+keyword+"&max-results=8&callback=?&alt=json&format=1";

  $.getJSON(apiURL,
    function(data){


      animationInit();
      clearScreen();
      hideEyes();
      showCenterPanel();
                var date;    
    var content = "";
                var month, day, year;


                //------------------------

 var $results =  $('#results');
         
          if(baseRef != null){
           var items = baseRef.itemsAmount;

           for(var i=0 ; i < items; i++){
              $results.data('owlCarousel').removeItem();
            }
            
          }else{
             $results.owlCarousel({
             stopOnHover : true,
             navigation:true,
             paginationSpeed : 200,
             goToFirstSpeed : 200,
             singleItem : true,
             transitionStyle:"fade"

              // navigation : true, // Show next and prev buttons
              // slideSpeed : 300,
              // paginationSpeed : 400,
              // singleItem:true
          });

           }

            for(var i = 0; i < 6; i++){

             content = "<div><img src='"+ data.feed.entry[i].media$group.media$thumbnail[0].url   
             + "' data-wow-"+i+"='"+ data.feed.entry[i].media$group.media$content[0].url +"'/></div>";
             //$results.append(content);
              $results.data('owlCarousel').addItem(content);

           } 
          


                //-------------------
      // for(var i = 0; i < 6; i++){
      //                   date = new Date(data.feed.entry[i].published.$t); 
      //                   month = date.getMonth()+1;
      //                   day = date.getDate();
      //                   year = date.getFullYear();
      // content = "<div class='pic not-selected' data-wow-index='"+i+"' >"+
      //                             "<a class='fancybox fancybox.iframe' href='"+data.feed.entry[i].media$group.media$content[0].url+"'>"+
      //                             "<img src='"+data.feed.entry[i].media$group.media$thumbnail[2].url + "'/>"+"</a></div>";
                                  
      //                          //   "<p>Published: "+day+"-"+month+"-"+year+"</p><br/>"+
                                
                          
      //   $('#results').append(content);
      // } 
                        
                      

                       
                       
    }
  );


}



///---------- tests -- module clavier 


function handleKeyboard(evt){

	console.info('handleKeyboard() - key: '+evt.keyCode);


	try{  
     megaMipJSInterface.onKeyBoard(evt.keyCode);
      }catch(error){
        console.log("bloc catch err: "+error);
      }
 
}

document.onkeyup = handleKeyboard;
