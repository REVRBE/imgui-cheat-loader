<h1>imgui cheat loader base + XenForo authentication</h1>

![mygif](https://user-images.githubusercontent.com/129604052/235543572-214e2e5b-763c-4eaf-b36d-e437ad765c99.gif)

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

4. You need to add this to the xf_user table in phpMyAdmin to make the sub-counter work. Run this SQL code:

```
ALTER TABLE xf_user
ADD COLUMN vip_end_time DATETIME DEFAULT NULL;

 // and then this after to update a single user's vip to 1 month, pretty simple to make a script that does this automaticly 

UPDATE xf_user
SET user_group_id = <VIP_group_id>, vip_end_time = DATE_ADD(NOW(), INTERVAL 1 MONTH)
WHERE user_id = <user_id>;
```

<h3>TO-DO on this project:</h3>
  ✅ Added XenForo support. (username, password & highest rank)
  
  ✅ Added nice colorways (Cinder + 6 custom ones)
  
  ✅ Add "days left" to rank VIP to track subscriptions. (currently has to manually add it to each VIP - next I'll create .php script for doing it automaticly.)
  
  ✅ Added TLS/SSL encryption
  
  ✅ Basic protection against SQL injection
    
  ❌ Add more customizations to the UI

<h3>Ways to avoid database breaches</h3>

Use a secure database password (it can be up to 128 characters - maybe even change it regularly)

Use a reliable/secure web-hosting

Add/make a script that notifies you with a mail to your personel/work mail-address when something fishy is going on in the database, so you can react quickly.

If you really want to go all-in, then look up Cloudflare WAF

<h3>Personal Note</h3>

I don't know why, but instead of going to sleep I produced this.... :P
