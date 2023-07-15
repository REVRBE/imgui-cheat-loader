<h1>imgui cheat loader base + XenForo authentication</h1>

![mygif](https://user-images.githubusercontent.com/129604052/235855892-db3e81d2-e405-4cb5-b940-488b3aa8df24.gif)

![15](https://user-images.githubusercontent.com/129604052/235858231-9499aefd-11c3-4e22-9c20-64a43c299629.PNG)

![16](https://user-images.githubusercontent.com/129604052/235858240-f2979213-082c-432c-9f59-01405428857b.PNG)
<h2>Usage</h2>
https://github.com/noteffex/ImGui-Loader-Base <- credits to this guy

Basicly this repo, but I've added XenForo authentication which check the user/pass from server-side and returns rank on succesful login e.g. Admin/VIP/other rank name from XenForo. This essentially is made for authenticating subscriptions by checking the rank of the forum-user. If the correct rank is returned, then access to load cheat is given.

It works with default installation of XenForo.

1. You need to put "login.php" file in your web-hotel (put it in standard XenForo folder) and update the database settings. 
2. Update a few lines of code and that should be all.
3. I've added prepared statements in the login.php file for basic protection against SQL injection - please update ".htaccess" with these lines of code:

```
SetEnv DB_SERVERNAME your_servername // change it to your database settings
SetEnv DB_USERNAME your_username
SetEnv DB_PASSWORD your_password
SetEnv DB_NAME your_db_name
```

4. You need to add this to the xf_user table in phpMyAdmin to make the sub-counter work (I'm soon releasing a version of the authentication script that also provide support for DragonByte Ecommerce). Run this SQL code:

```
ALTER TABLE xf_user
ADD COLUMN vip_end_time DATETIME DEFAULT NULL;

 // and then this after to update a single user's vip to 1 month, pretty simple to make a script that does this automaticly 

UPDATE xf_user
SET user_group_id = <VIP_group_id>, vip_end_time = DATE_ADD(NOW(), INTERVAL 1 MONTH)
WHERE user_id = <user_id>;

 // and also add this for extra sql injection protection
 
 USE your_database_name; // insert your database name

CREATE TABLE xf_login_attempts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    login_attempts INT NOT NULL DEFAULT 0,
    last_login_attempt TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (username)
);
```

<h3>TO-DO on this project:</h3>
  ✅ Added XenForo support. (username, password & highest rank)
  
  ✅ Added nice colorways (Cinder + 13 custom colorways now!)
  
  ✅ Added "days left" to rank VIP to track subscriptions. (currently has to manually add it to each VIP - next I'll create .php script for doing it automaticly.)
  
  ✅ Added TLS/SSL encryption
  
  ✅ Added medium protection against SQL injection
  
  ✅ Added so it remembers last color scheme used

  ❌ Add support for DragonByte Ecommerce add-on
  
  ❌ Add so it specifies how many login tries the user has left before cooldown.

  ❌ Re-write imgui base so it's DX11 instead of DX9.

<h3>Ways to avoid database breaches</h3>

Use a secure database password (it can be up to 128 characters - maybe even change it regularly)

Use a reliable/secure web-hosting

Add/make a script that notifies you with a mail to your personel/work mail-address when something fishy is going on in the database, so you can react quickly.

If you really want to go all-in, then look up Cloudflare WAF

<h3>Update</h3>

I added another version of 'xenforo.hh' which has DragonByte support. Currently it's able to return all licenses; their expiry date, license id and product id. I will update the 'gui.cc' when I have some spare time, so it uses the returned data to display it. 

<h3>DX11 version for sale</h3>

Check https://www.youtube.com/watch?v=NhSRl99HXaM for more information

https://github.com/REVRBE/imgui-cheat-loader/assets/129604052/bc8d41a5-05a0-4d6e-af67-687ac1bf8775

(sorry for bad video quality, I had to compress it in order to get it under 10MB)
