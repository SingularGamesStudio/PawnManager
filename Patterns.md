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

#### �㡨��� ��ᨫ��:

�ࢥୠ� ����(����� net)

Connection - Adapter �⥢�� ������⥪� (��� �ࢥ�/������)

clientInterface/serverInterface - Interface ��� Connection

#### �����஢ ����(vertignesss):

Pawn + ��᫥�����

FighterPawn - Factory


