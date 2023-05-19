# Паттерны использованные в проекте + кто что делал

#### Кирьянов Григорий(SingularGamesStudio):

Building, Recipe, Entity, Player, IDmanager

Entity - Composite

Player::TaskManager - Proxy между Pawn и Player.

Player - Facade между реальным игроком и Recipe

Recipe - Builder, Prototype

Player::TaskManager::PendingRecipe - Command

IDmanager - Singleton

#### Косов Михаил(128-bit-guy):

Графический интерфейс(папка GUI)

BuildingRenderer, FontManager, GameWindowManager, PawnRenderer, ResourceRenderer - Singleton'ы

#### Дубинин Василий(Dubvas):

серверная часть(папка net)

Connection - Adapter сетевой библиотеки (для сервера/клиента)

clientInterface/serverInterface - Interface для Connection

события Events - оповещения от сервера клиенту о любых изменениях(Factory для Packet)

serialization/deserialization - байтовое кодирование всех интересующих нас
классов(Pawn, Building, Player, Recipe)

#### Тамьяров Илья(vertignesss):

Pawn + наследники

FighterPawn - Factory


