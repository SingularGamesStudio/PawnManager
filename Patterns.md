# ������ �ᯮ�짮����� � �஥�� + �� �� �����

#### ����ﭮ� �ਣ�਩(SingularGamesStudio):

Building, Recipe, Entity, Player

Entity - Composite

Player::TaskManager - Bridge ����� Pawn � Player.

Player - Facade ����� ॠ��� ��ப�� � Recipe

Recipe - Command (�� ����� ��� Pawn/Building)

#### ��ᮢ ��堨�(128-bit-guy):

����᪨� ����䥩�(����� GUI)

BuildingRenderer, FontManager, GameWindowManager, PawnRenderer, ResourceRenderer - Singleton'�

#### �㡨��� ��ᨫ��(Dubvas):

�ࢥୠ� ����(����� net)

Connection - Adapter �⥢�� ������⥪� (��� �ࢥ�/������)

clientInterface/serverInterface - Interface ��� Connection

ᮡ��� Events - �����饭�� �� �ࢥ� ������� � ���� ����������(Factory ��� Packet)

serialization/deserialization - ���⮢�� ����஢���� ��� ��������� ��� 
 ����ᮢ(Pawn, Building, Player, Recipe) 

#### �����஢ ����(vertignesss):

Pawn + ��᫥�����

FighterPawn - Factory


