#Паттерны использованные в проекте.
####Кирьянов Григорий(SingularGamesStudio):
Entity - Composite

Player::TaskManager - Bridge между Pawn и Player.

Player - Facade между реальным игроком и Recipe

Recipe - Command (из задач для Pawn/Building)

####Косов Михаил(128-bit-guy):
BuildingRenderer, FontManager, GameWindowManager, PawnRenderer, ResourceRenderer - Singleton'ы

Control, Window - 

####Дубинин Василий:

Connection - Адаптер сетевой библиотеки (для сервера/клиента)

clientInterface/serverInterface - Interface для Connection

####Тамьяров Илья(vertignesss):
FighterPawn - AbstractFactory

