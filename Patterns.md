#������ �ᯮ�짮����� � �஥��.
####����ﭮ� �ਣ�਩(SingularGamesStudio):
Entity - Composite

Player::TaskManager - Bridge ����� Pawn � Player.

Player - Facade ����� ॠ��� ��ப�� � Recipe

Recipe - Command (�� ����� ��� Pawn/Building)

####��ᮢ ��堨�(128-bit-guy):
BuildingRenderer, FontManager, GameWindowManager, PawnRenderer, ResourceRenderer - Singleton'�

Control, Window - 

####�㡨��� ��ᨫ��:

Connection - ������ �⥢�� ������⥪� (��� �ࢥ�/������)

clientInterface/serverInterface - Interface ��� Connection

####�����஢ ����(vertignesss):
FighterPawn - AbstractFactory

