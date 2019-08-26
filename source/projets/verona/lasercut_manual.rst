Manual to use Laser Machine from VERONA Fablab
==============================================


**Before using laser :**

- Need to identify each part of the drawing that need different operation (Cut / Engrave / Mark)
- Put each operation on a different layer
- Export your job as DXF files

**The different operation possible :**

- Cut : the laser will follow drawing/trajectory to cut through material
- Engrave : Laser will fill a surface/shape to burn surface of material
- Mark : Laser will follow drawing/trajectory to burn surface on material

In the laser Software, Cut and Mark will be considered as the same job, only parameters will be differents, where Mark will be less powerful than Cut.


On the laser part, preparation on software “LaserWork V6” :
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


.. figure :: ../../_static/verona-laser/im1.png

  Import DXF file previously generated

.. figure :: ../../_static/verona-laser/im2.png

  All layers of the file shall be differentiated by colors in the “work” tab

.. note::

  The position of the job doesn’t matter

**Verify if the dimension of the job are respected**

- select the job in the software, draws appear on blue on the window

.. figure :: ../../_static/verona-laser/im3.png

  verify the dimension of it (in red), change the dimension if needed, and respect proportion in needed (in blue)

**FOR ARRANGING JOB**

.. figure :: ../../_static/verona-laser/im4.png

  If needed reproduce the job with “matrix copy” (in RED).

  Choose the number of iteration of the job along X&Y Axis (in BLUE) and space between each iteration along X&Y Axis (in GREEN)

.. figure :: ../../_static/verona-laser/im5.png

  If doing a “matrix copy”, if no x&y width, there will be overlap, you can “delete overlap” to avoid the laser to pass twice on the same path

.. figure :: ../../_static/verona-laser/im6.png

  Rotate the job (Iin GREEN) or mirror it (in RED)

*The start point will determine where the laser head will start the job (displayed as the green square in the following image). You can configure the start point.*

.. figure :: ../../_static/verona-laser/im7.png

  config -> system setting

.. figure :: ../../_static/verona-laser/im8.png

  Select where laser head will start in the “Laser Head” menu (in RED), this can be change regarding the material you have

**Configure operations parameters**


.. csv-table:: Table with laser parameters regarding the material used
     :file: ../../_static/verona-laser/laser_param.csv
     :widths: 17,17,17
     :header-rows: 1

To configure CUT
++++++++++++++++

.. figure :: ../../_static/verona-laser/im9.png

  Double click on a “CUT” opperation in “Work” tab

.. figure :: ../../_static/verona-laser/im10.png

* you can select the type of opperation with the processing mode (in PURPLE)
* put “YES” for “is OUTPUT” to validate the operation (in RED)
* put laser speed (in BLUE)
* put laser power min / max (with not more than 4 as difference) (in GREEN)
* For laser parameters, use data regarding the material used
* put “blowing” as YES (in YELLOW)

To configure SCAN
+++++++++++++++++

.. figure :: ../../_static/verona-laser/im11.png

* put “YES” for “is OUTPUT” to validate the operation (in RED)
* put laser speed (in BLUE)
* put laser power min / max (with not more than 4 as difference) (in YELLOW)
* For scanning, go as fast as possible (around 100 for laser speed) and with low parameters, around 10 is enough for wood engraving.
* put “blowing” as NO (in GREEN), this shall avoid accumulation of dust on material
* select scan mode (in PURPLE)
* swing mode will spend less time moving laser head
* unilateralism mode will start each line of engraving from the same side, will take more time
* select independent output, so engraving will do one shape after another, and quality will be better (in PINK)
* Choose laser interval(width) that will impact engraving aspect, by default is 0.1mm (in ORANGE)

.. figure :: ../../_static/verona-laser/im12.png

  In the work tab, order the operation with a preference for SCAN before CUT

.. figure :: ../../_static/verona-laser/im13.png

  Verify consistency of the job with simulation

.. figure :: ../../_static/verona-laser/im14.png

  Verify consistency of the job with simulation

Time to use laser !!
++++++++++++++++++++

**Open the laser lid**
**install material on the laser bed**

.. figure :: ../../_static/verona-laser/im15.png

.. figure :: ../../_static/verona-laser/im16.png

  Turn on laser machine with RED button
  Turn ON laser with GREEN button

.. figure :: ../../_static/verona-laser/im17.png

  Turn ON ventilation

.. figure :: ../../_static/verona-laser/im18.png

  move the laser head with arrows, take in account the start point of the job defined before

**settle the laser focus (laser bed height) with 5mm stake**

.. figure :: ../../_static/verona-laser/im19.png

  on laser dashboard press “Z/U” button (in RED)

.. figure :: ../../_static/verona-laser/im20.png

  move bed height with left/right arrows so stake fill gap between laser head and material

**remove stake**

.. figure :: ../../_static/verona-laser/im21.png

  check size of the job with *Go Scale*

**close laser lid**

.. figure :: ../../_static/verona-laser/im22.png

  start the job

A different material setup to avoid burn at the bottom (in RED)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. figure :: ../../_static/verona-laser/im23.png

.. figure :: ../../_static/verona-laser/im24.png

  add screws on the bed

.. figure :: ../../_static/verona-laser/im25.png

  put the material on the bed

**adjust accordingly z height**
