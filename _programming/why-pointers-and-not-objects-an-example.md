---
layout: page
title: "Why Pointers and Not Objects? An Example"
---

A common question is, *Why do we use pointers as opposed to objects for attributes or in collections?* First, we typically want polymorphic behavior and the only way to achieve that in C++ is by using pointers. However, an inheritance relationship is not nearly as common as a use relationship and we often use pointer variables to track the things associated to a class.

Objects in our system represent real-world entities. If these objects need to be used in various parts of the system, we have two ways to handle it:

- Use object variables and have multiple instances of the same real-world object in our system
- Use pointer variables so that all parts share a common object

As the system progresses, updates to the objects will take place (e.g., an employee needs a name change) and that change has to be observed throughout the system. If we use object variables, we have to know where they are in the system and make sure each one is updated. When using pointers, we update the shared object and all users will observe the change.

To help illustrate this, we will use a simple HR System Application (C++ implementation: [HRApp](https://drive.google.com/file/d/1KP3Fms0nQ1yffIVKBJl4g0pgOH0qYevU/view?usp=drive_link)). The main system interface is managed by the class `HumanResourcesApp` which interacts with an `EmployeeDatabase`, the thing that manages all of the `Employee` objects in the system, and a `TeamManager`, the thing that manages all the `Team` objects in the system. The relationship between these classes can be seen in the UML diagram in Figure 1.

<figure class="numbered">
  <img src="/assets/programming/why-pointers-and-not-objects-an-example/hr_uml_diagram.jpg"
       alt="The UML class diagram for the HR App system.">
  <figcaption>
    The UML class diagram for the HR App system.
  </figcaption>
</figure>

Pointers are used in a number of places, but there are ones to note. First, `EmployeeDatabase` manages the pointer array, `Employee** employees`. We can’t use an object array because `Employee` doesn’t have a default constructor. That is, if we tried

```cpp
Employee* employee;
```

when we tried to allocate the array, as in

```cpp
employee = new Employee[EMPLOYEE_STEP];
```

a compiler error would occur since there is no parameterless constructor available.

Second, `TeamManager` manages the pointer array, `Team** teams`. This is an analogous situation to the what was described for `EmployeeDatabase` and `Employee** employees`.

Third, `Employee` manages the pointer array, `Team** teams`. These are the references to the teams that the employee belongs to. As there are times when an `Employee` object needs to inquire about something from a team and changes to the team could occur, the `Employee` needs to interact with the actual `Team` object, not a copy.

Lastly, `Team` manages the pointer array, `Employee** staff`, as well as the pointer, `Employee* manager`. As there are times when an `Team` object needs to inquire about something from the manager or a staff member and changes to the any of those objects could occur, the `Team` needs to interact with the actual `Employee` objects, not copies.

As you inspect the code, another thing to observe is how the bidirectional relationship between `Employee` and `Team` is managed. There will never be a situation where an employee thinks it’s on a team but the team doesn’t know of the employee - and vice versa. Ensuring objects are connected this way can sometimes take a good bit coordination.


### Thing to Know Before Running the HR App System
The system is a bit fragile. While I tried to make it reasonably robust, I am sure there are some situations that might cause the program to behave oddly or even crash. Once the system is up an running, it should be pretty easy to navigate. If you want to create a team with new employees (once a team is created you can’t add employees to it ... though you could add that feature):

- Add an employee as a manager
- Find the employee by name and copy their id
- Find any other employees by name you want on the team and copy their ids
- Create a team using the manager id and the staff ids

When you start the application, you will be prompted to load an employee data file and then a team data file. If you choose `'n'` for both, you will start with no employess or teams. If you want to load some employees and teams, you can use “employee.txt” and “team.txt”. You can’t load the team file until you load the employee file as teams are only valid if the employee ids referenced are valid.

Once the system is up an running it should be pretty clear what to do. You will be able to add, edit (name only), find and remove employees and teams. A few things to note:

- An employee has a first and last name. Neither can contain spaces.
- A team name can contain spaces.

Lastly, many of the menu actions require an employee or team id. You can find the ids by searching for the employee or team by name. If you use the data files, you can find ids in those files. 

The employee file contains employee information and consists of blocks of the form (the dash is important!):

```
-
<employe id>
<employee first name - no spaces> <employee last name - no spaces>
<manager flag - true = is a a manager, false = not a manager>
```

For example, 

```
-
912345678
Tom Jones
true
-
987654321
BillyDee Williams
false
-
998877665
Isaac Hayes
false
```

The team file contains team information and consists of blocks of the following form (the dash is important!):

```
-
<team id>
<team name - may have spaces>
<manager id>
<staff id>
. . .
<staff id>
```

where there must be a manager id (the id of a manager in the employee database) and a set of staff ids that are on the team staff (the set may be empty). 

Here is an example file with a team with a a manager and no staff and a team with a manager and three staff members:

```
-
812345679
Empty Team
912345678
-
897654321
Non-Empty Team
912345678
987654321
998877665
911223344
```

To delete a manager requires deleting all the teams the manager manages.

If you exit the system by selecting "Quit" from the main menu, you can save any changes you make. I would recommend not overwriting the original data files so that you don’t have to rebuild them or hunt them down again.

Honestly, there are things I would probably do differently if I were building a real system. For example, the `EmployeeDatabase` has operations to load/save the `Employee` data whereas the loading/saving for the `Team` data is handled in `HumanResourcesApp` rather than `TeamManager`. If I were to refactor this example, one thing on the to-do list would be to fix that. The reason I went the direction I did is because making that change would introduce a dependency between `TeamManager` and `EmployeeDatabase`. I decide to avoid that, but it might have been the better approach.

You will likely find the interface a bit limited. Creating interfaces gives me very little joy. I find them tedious to build because you have to think of the bazillion ways user can use your system incorrectly. I’m more of system design and algorithms guy. You could always make improvements.


















