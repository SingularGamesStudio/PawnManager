# Паттерны использованные в проекте + кто что делал

#### Кирьянов Григорий(SingularGamesStudio):

Building, Recipe, Entity, Player

Entity - Composite

Player::TaskManager - Bridge между Pawn и Player.

Player - Facade между реальным игроком и Recipe

Recipe - Command (из задач для Pawn/Building)

#### Косов Михаил(128-bit-guy):

Графический интерфейс(папка GUI)

BuildingRenderer, FontManager, GameWindowManager, PawnRenderer, ResourceRenderer - Singleton'ы

#### Дубинин Василий:

серверная часть(папка net)

Connection - Adapter сетевой библиотеки (для сервера/клиента)

clientInterface/serverInterface - Interface для Connection

#### Тамьяров Илья(vertignesss):

Pawn + наследники

FighterPawn - Factory


