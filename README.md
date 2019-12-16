Genpool

Maja Gavrilovic 489/2017 majamaj@live.com
Aleksandra Boskovic 263/2018 aleksandra94@hotmail.com
Marica Bogicevic 427/2015 tvorac22@gmail.com

Use different libraries like Gl/glut.h and png++/png.h

project:
  refrences:
    - https://www.youtube.com/watch?v=3mgGQR6oGxI
    - https://www.youtube.com/watch?v=t73z0fzxMlE
  short description: |
    cellular automata simulator engine with easily programmable rules and conditions.
  definitions:
    property: |
      property is defined with an image. On an image is a "heat map"
      of the property such as heat maps from geography or height maps, iq maps,
      gini coeficient and other interesting properties bound to our planet,
      and/or psycho/socialogical caracteristics.
      More strictly said, mapping from 2D to one. From R^2 -> R.
      This static caracteristic of the terrain.
    population-cell: |
      This is what we simulate. Each pixel may represent a population or a single
      cell. Population aproach may be more interesting.
    Artributes: |
      Each population/cell will have a set of attributes which are dynamic -
      meaning they change between populations as they come and go but may also
      be change in single population it self. (these are 2 distinct features)
    modes:
      desc: The program has 3 modes.
      inductive-mode: | 
        Set of properties is given as well as set of rules. Then the simulation
        is played out in gui. Project will start this mode and maybe other
        modes will be added. Most probably not - hence the short description,
        otherwise that desc would be too narrow.
      deductive-mode: |
        Two set of properties are given. Lets call one set simple properties,
        and the other complex properties. Complex properties are computed
        form simple properties. These may be devided in more then 2 sets/levels;
        This defines a fully connected weighted graph - and these weight may
        be computed in a neural network.
        So imput to deductive mode are only properties and result are rules.
        This mode probably won't be done but is part of project description.
      hybrid-mode: |
        If there is no deductive mode, then there is no hybid mode.
        Hybrid mode would be dynamic aproach of modeling in given area of interest,
        which has its basis in both statistical knowlage and in idea which may
        guide and introduce "rapid tuning" and guide the results.
  stories:
    property definition: 
      priority: 5
      description: |
        it would be nice if we had an editor for our properties but this is not
        primary goal of the project. Initial maps will be created using photoshop
        and maybe later this feature will be added.
      dependency: gui module, image manipulation program/module = IMP
    Blending:
      priority: 1
      desc: |
        End user may blen any of starting properties with the simulation for better
        inspection of the results
    Attribute visualisation:
      priority: 1
      desc: |
        Attributes should be visualised by pixel color. User should be able to
        control which attributes are visualised.
    Controls:
      priority: 2
      End user may pause, speed up and speed down a simulation
    Analitics:
      priority: 2
      User should be able to see analytics of the current simulation.
    Developer story: |
      Easy intuitive way of definig new rules over any set of heat maps
      which will provide visual feedback in end user story
      Requires nice design patten to solve this
  modules:
    gui module: priority 2
    image manipulation module: priority 2
    heat map editor:  priority 3
    Property manager:
      decoder: priority 1
      encoder: priority 1
    Simulator:
