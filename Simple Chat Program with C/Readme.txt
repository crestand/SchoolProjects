Kodları compile etmek için 
gcc -pthread server.c -o server
gcc -pthread client.c -o client

komutlarını kullanınız.


Server'ı çalıştırmak için ->
1) Terminalde server'in bulunduğu dizine gelin
2) İstediğiniz portu kullanarak "./server [PORT]" komutuyla serveri başlatın.
3) Kullanıcıların bulunduğu "users.txt" dosyası da aynı dizinde bulunmalıdır.


Client'ları başlatmak için ->
1) Terminalde client'in bulunduğu dizine gelin
2) Bağlanmak istediğiniz server'in portunu kullanarak "./client [PORT]" komutuyla client'ı başlatın.
3) Kullanıcı adını ve şifrenizi girin
4) Aktif kullanıcıları görmek için "get_clients" komutunu kullanabilirsiniz.
5) Aktif bir kullanıcıya mesaj göndermek için "kullanıcı_adı/mesaj" formatınında mesaj gönderebilirsiniz.
6) Server'dan ayrılmak için "exit" yazabilir ya da CTRL+C kısayolunu kullanabilirsiniz. 
