


// methods to display goDog results 


var data2 = {"poi":[{"zip":"H7G 2T9","reviews":"2","distance":"2164.5100950207334","phone":"450-663-8828","address":["420 Boul Des Laurentides"],"name":"Bravo Pizzaria","state":"QC","image":"http:\/\/s3-media2.ak.yelpcdn.com\/bphoto\/DAx-kHmOWPfhSITHOeSj_w\/ms.jpg","rating":"1.5","url":"http:\/\/www.yelp.ca\/biz\/bravo-pizzaria-laval","city":"Laval"},{"zip":"H7N 2H5","reviews":"0","distance":"1821.7723983696098","phone":"450-663-4545","address":["62 Boul Cartier O"],"name":"Surprise Pizza 2 Pour 1","state":"QC","rating":"0","url":"http:\/\/www.yelp.ca\/biz\/surprise-pizza-2-pour-1-laval","city":"Laval"},{"zip":"H7N 2H5","reviews":"0","distance":"1821.7723983696098","phone":"514-761-7761","address":["62 Cartier Boul W"],"name":"Kurde Pizza","state":"QC","rating":"0","url":"http:\/\/www.yelp.ca\/biz\/kurde-pizza-laval-2","city":"Laval"},{"zip":"H7N 2H5","reviews":0,"distance":"1821.7723983696098","phone":"514-761-7761","address":["62 Boul Cartier O"],"name":"Kurde Pizza","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/kurde-pizza-laval","city":"Laval"},{"zip":"H2C 1X9","reviews":0,"distance":1653.969387651868,"phone":"514-858-1991","address":["1799 Rue Sauvé E"],"name":"Pizza Florencia","state":"QC","image":"http:\/\/s3-media3.ak.yelpcdn.com\/bphoto\/R1xJ7PYK-bCDIoKJXiTHUQ\/ms.jpg","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/pizza-florencia-montr%C3%A9al-3","city":"Montréal"},{"zip":"H3L 1V3","reviews":0,"distance":1570.4068376961384,"phone":"514-381-5991","address":["312 Fleury Rue W"],"name":"Rino Pizzeria","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/rino-pizzeria-montreal","city":"Montreal"},{"zip":"H3L 2A5","reviews":1,"distance":1520.1570877261586,"phone":"514-383-4567","address":["11 De Port-Royal Rue W"],"name":"Domino's Pizza","state":"QC","image":"http:\/\/s3-media1.ak.yelpcdn.com\/bphoto\/mIGcXIwJ9iAYmnpodg1uVA\/ms.jpg","rating":4,"url":"http:\/\/www.yelp.ca\/biz\/dominos-pizza-montreal-6","city":"Montreal"},{"zip":"H3L 2M9","reviews":0,"distance":1519.6409549264017,"phone":"514-383-4567","address":["11 Rue de Port-royal O"],"name":"Domino's Pizza","state":"QC","image":"http:\/\/s3-media3.ak.yelpcdn.com\/bphoto\/A1FHC8yTvdaIftep8yNpYw\/ms.jpg","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/dominos-pizza-montr%C3%A9al-15","city":"Montréal"},{"zip":"H2M 1S5","reviews":3,"distance":1478.7933219027689,"phone":"514-388-9672","address":["9481 Lajeunesse Rue"],"name":"Martini Pizzeria","state":"QC","image":"http:\/\/s3-media4.ak.yelpcdn.com\/bphoto\/zvcowykS6jo65yU5S1Zmdg\/ms.jpg","rating":3.5,"url":"http:\/\/www.yelp.ca\/biz\/martini-pizzeria-montreal","city":"Montreal"},{"zip":"H3L 2N3","reviews":0,"distance":1357.9146195505161,"phone":"514-343-0343","address":["9870 St-laurent"],"name":"Double Pizza","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/double-pizza-montr%C3%A9al-13","city":"Montréal"},{"zip":"H3L 2N3","reviews":0,"distance":1357.9146195505161,"phone":"514-388-0123","address":["9870 Saint-Laurent Boul"],"name":"Double Pizza","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/double-pizza-montreal-7","city":"Montreal"},{"zip":"H3L 1N3","reviews":0,"distance":1245.966639264822,"phone":"514-332-1332","address":["196 Henri-Bourassa Boul W"],"name":"Pizza Roni","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/pizza-roni-montreal","city":"Montreal"},{"zip":"H2C 1Z2","reviews":0,"distance":1041.9862449264526,"phone":"514-387-9321","address":["844 Sauve Rue E"],"name":"Restaurant Georges","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/restaurant-georges-montreal","city":"Montreal"},{"zip":"H2C 1Z2","reviews":0,"distance":1041.9862449264526,"phone":"514-387-9321","address":["844 Sauve Rue E"],"name":"Georges Pizza","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/georges-pizza-montreal","city":"Montreal"},{"zip":"H2C 2M9","reviews":0,"distance":607.3458174001971,"phone":"514-850-0858","address":["1155 Fleury E"],"name":"Pizza 4 Freres","state":"QC","rating":0,"url":"http:\/\/www.yelp.ca\/biz\/pizza-4-freres-montr%C3%A9al","city":"Mont"}]};

function showMyVideos(){


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

            
          });

          }   

            for(var i = 0; i < 2; i++){

             content = "<div><img src='img/vid0"+i+".png' data-wow-"+i+"="+"'vid0"+i+"' /></div>";
             //$results.append(content);
              $results.data('owlCarousel').addItem(content);

           } 

         
}


function showMyPictures(){


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

            
          });

          }   

            for(var i = 0; i < 2; i++){

             content = "<div><img src='img/pic0"+i+".jpg'  /></div>";
             //$results.append(content);
              $results.data('owlCarousel').addItem(content);

           } 

         
}



function changeLocation(newLocation){


   var ofst = offset.montreal;
   mLocation = 'montreal';
   if(newLocation == "tokyo"){
   	ofst = offset.tokyo;
   	mLocation = 'tokyo';
   } 
   if(newLocation == "san_francisco"){
   	mLocation = 'san fran.';
   	ofst = offset.san_francisco;
   } 

  
   mOffset = ofst;
    var $location = $('#location');
    $location.html(mLocation);
    displayDate2();
 }

 function getMipDate(){
   var d = new Date();
   var utc = d.getTime() + (d.getTimezoneOffset() * 60000);
   var nd = new Date(utc + (3600000*mOffset));
   
  return nd;
}

function updateResult() {
			// var result = JSON.parse(data2);



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

          // for(var i = 0; i < 6; i++){

          //    content = "<div><img src='"+json.responseData.results[i].url + "'/></div>";
          //    //$results.append(content);
          //     $results.data('owlCarousel').addItem(content);

          //  } 

//------------------------------


			var result = data2;
			var userID = "mip@godogfetch.com";
			var userPWD = "godoglabs";
			var latlong = "";
			
			result.poi = (result.poi || []);
			result.img = (result.img || []);
			result.url = (result.url || '');
			result.map = (result.map || '');
			
	        if (result.context === 'location') {
	            if (result.lat && result.lng) {
					var url = "https://labs.godogfetch.com/apps/MapIt/MapIt.html?user=" + userID + "&password=" + userPWD + "&lat=" + result.lat + "&long=" + result.lng;
					var width = $(window).width();
					var item = "<iframe src=\"" + url + "\" width=\"" + width + "\" height=\"500\"></iframe>";
					$('#listview').append('<br />' + item).trigger('create');
				}
			} else {
				if (result.poi.length === 0 && result.img.length === 0) {
//					if (result.url !== '') {
//						window.open(result.url, '_blank');
//					} else if (result.map && result.map !== '') {
//						window.open(result.map, '_blank');
//					}
				}
				
                for (i = result.poi.length - 1; i >= 0; --i) {
                    var locationIndex = (result.poi.length - i);
                    var name = (result.poi[i].name || '');
                    var address = (result.poi[i].address || '');
                    var city = (result.poi[i].city || '');
                    var state = (result.poi[i].state || '');
                    var zip = (result.poi[i].zip || '');
                    var phone = (result.poi[i].phone || '');
                    var distance = (result.poi[i].distance || 0);
                    var rating = (result.poi[i].rating || 0);
                    var reviews = (result.poi[i].reviews || 0);
                    var image = (result.poi[i].image || '');
                    var url = (result.poi[i].url || '');

                    var physicalAddr = address + ' ' + city + ' ' + state + ' ' + zip;

                    miles = distance * 0.000621371;
                    distance = miles.toString();
                    m = distance.indexOf('.');
                    if (m != -1) {
                        distance = distance.slice(0, m + 2);
                    }
					
					var item = 
						"<div class=\"list\">" +
							"<div class=\"list-content\">" +
				                "<a href=\"{URL_SITE}\" target=\"_blank\"><img src=\"{URL_IMG}\" class=\"icon\" width=\"62\" height=\"62\"></a>" +
				                "<div class=\"data\">" +
									"<address>" +
										"<strong><large>{NAME}</large></strong><br />" +
										"{ADDR1}<br />" +
										"{ADDR2}<br />" +
										"{PHONE}<br />" +
										"{DISTANCE}<br />" +
									"</address>" +
									"<img src=\"{STAR_RATING}\">" +
				                    "<p class=\"tertiary-text fg-blue\" style=\"font-size: 16px;\">{REVIEWS}</p><br />" +
									"<a href=\"{MAP}\" target=\"_blank\"><i class=\"icon-location fg-blue on-left-more\" style=\"font-size: 22px; {MAP_DISPLAY}\"></i></a>" +
									"<a href=\"{NAV}\" target=\"_blank\"><i class=\"icon-location-2 fg-blue on-right-more on-left-more\" style=\"font-size: 22px; {NAV_DISPLAY}\"></i></a>" +
									"<a href=\"tel:{URL_PHONE}\" target=\"_blank\"><i class=\"icon-phone fg-blue on-right-more on-left-more\" style=\"font-size: 22px; {PHONE_DISPLAY}\"></i></a>" +
									"<a href=\"{URL_SITE}\" target=\"_blank\"><i class=\"icon-globe fg-blue on-right-more on-left-more\" style=\"font-size: 22px; {URL_DISPLAY}\"></i></a>" +
								"</div>" +
			                "</div>" +
			            "</div>";
						
					var locationUrl = encodeURIComponent(address + ' ' + city + ' ' + state + ' ' + zip);
					var callUrl     = phone.replace(/[^0-9]/g, '');
						
					item = item.replace(/{NAME}/g, (result.poi.length - i) + '. ' + name);
					item = item.replace(/{URL_SITE}/g, url);
					item = item.replace(/{URL_IMG}/g, (image !== '' ? image : 'https://labs.godogfetch.com/img/no-image-avatar.jpg'));
					item = item.replace(/{ADDR1}/g, address);
					item = item.replace(/{ADDR2}/g, city + ', ' + state + ' ' + zip);
					item = item.replace(/{PHONE}/g, phone);
					item = item.replace(/{URL_PHONE}/g, callUrl);
					item = item.replace(/{DISTANCE}/g, distance + ' mi');
					item = item.replace(/{REVIEWS}/g, reviews + ' reviews');
					item = item.replace(/{MAP}/g, 'https://maps.google.com/maps?q=' + address + ' ' + city + ' ' + zip);
					item = item.replace(/{NAV}/g, 'https://maps.google.com/maps?saddr=' + latlong + '&daddr=' + locationUrl);
					
					item = item.replace(/{MAP_DISPLAY}/g, (address === '') ? 'display: none;' : '');
					item = item.replace(/{NAV_DISPLAY}/g, (address === '') ? 'display: none;' : '');
					item = item.replace(/{PHONE_DISPLAY}/g, (callUrl === '') ? 'display: none;' : '');
					item = item.replace(/{URL_DISPLAY}/g, (url === '') ? 'display: none;' : '');
					
					rating = Math.round(rating / 0.5) * 0.5;
					switch (rating) {
						case 0.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_0.0.png');
							break;
							
						case 0.5:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_0.5.png');
							break;
							
						case 1.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_1.0.png');
							break;
							
						case 1.5:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_1.5.png');
							break;
							
						case 2.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_2.0.png');
							break;
							
						case 2.5:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_2.5.png');
							break;
							
						case 3.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_3.0.png');
							break;
							
						case 3.5:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_3.5.png');
							break;
							
						case 4.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_4.0.png');
							break;
							
						case 4.5:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_4.5.png');
							break;
							
						case 5.0:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_5.0.png');
							break;
							
						default:
							item = item.replace(/{STAR_RATING}/g, 'https://labs.godogfetch.com/img/stars_0.0.png');
							break;
					}
					
					//$('#listview').append(item).trigger('create');
					  $results.data('owlCarousel').addItem(item);
				}  //---------
				
				for (i = 0; i < result.img.length; ++i) {
					if (result.img[i]) {
						$('#listview').append('<p>' + result.img[i] + '</p>').trigger('create');
					}
				}
			}
			
			if (result.html) {
				for (i = 0; i < result.html.length; ++i) {
					$('#listview').append(result.html[i]).trigger('create');
				}
			}
		}



		function calcTime(city, offset) {
    // create Date object for current location
    var d = new Date();

    // convert to msec
    // add local time zone offset
    // get UTC time in msec
    var utc = d.getTime() + (d.getTimezoneOffset() * 60000);

    // create new Date object for different city
    // using supplied offset
    var nd = new Date(utc + (3600000*offset));

    // return time as a string
    return "The local time for city"+ city +" is "+ nd.toLocaleString();
}