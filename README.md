
### Get this product for $5

<i>Packt is having its biggest sale of the year. Get this eBook or any other book, video, or course that you like just for $5 each</i>


<b><p align='center'>[Buy now](https://packt.link/9781783987948)</p></b>


<b><p align='center'>[Buy similar titles for just $5](https://subscription.packtpub.com/search)</p></b>


# OpenFlow Cookbook 

<a href="https://prod.packtpub.com/in/networking-and-servers/openflow-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781783987948"><img src="https://prod.packtpub.com/media/catalog/product/cache/ecd051e9670bd57df35c8f0b122d8aea/7/9/7948os_openflow20cookbook.jpg" alt="OpenFlow Cookbook " height="256px" align="right"></a>

This is the code repository for [OpenFlow Cookbook ](https://prod.packtpub.com/in/networking-and-servers/openflow-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781783987948), published by Packt.

**Over 110 recipes to design and develop your own
OpenFlow switch and OpenFlow controller**

## What is this book about?
Moving on from the traditional, configuration-driven network, OpenFlow paves the way to an open, centrally programmable structure providing standard interfaces between the controller and the actual packet forwarding entity.

This book has been divided into two parts to provide you with everything you need to know to develop either an OpenFlow switch or an OpenFlow controller. Starting from the basics of establishing communication channels between the controller and switch, this book explains in detail about the various tables, table operations, and switch and controller procedures.

This book covers the following exciting features:
* Create, maintain, and close an OpenFlow communication channel between the switch and controller
* Manage multiple switches from a single controller and vice versa: manage a single switch from multiple controllers with different controller roles
* Configure an OpenFlow switch using standard OpenFlow controller and switch procedures
* Explore tables present in OpenFlow switches such as flow tables, group tables, and meter tables
* Using controller and switch procedures, program tables within the switch, such as flow tables, group tables, and meter tables

If you feel this book is for you, get your [copy](https://www.amazon.com/dp/1783987944) today!

<a href="https://www.packtpub.com/?utm_source=github&utm_medium=banner&utm_campaign=GitHubBanner"><img src="https://raw.githubusercontent.com/PacktPublishing/GitHub/master/GitHub.png" 
alt="https://www.packtpub.com/" border="5" /></a>

## Instructions and Navigations
All of the code is organized into folders. For example, Chapter02.

The code will look like the following:
```
/* Switch features. */
struct ofp_switch_features {
struct ofp_header header;
uint64_t datapath_id; /* Datapath unique ID. The lower 48-bits are
for a MAC address, while the upper
16-bits are implementer-defined. */
uint32_t n_buffers; /* Max packets buffered at once. */
uint8_t n_tables; /* Number of tables supported by datapath. */
uint8_t auxiliary_id;/* Identify auxiliary connections */
uint8_t pad[2]; /* Align to 64-bits. */
/* Features. */
uint32_t capabilities; /* Bitmap of support "ofp_capabilities". */
uint32_t reserved;
```
## Get to Know the Author
**Kingston Smiler S**
is a seasoned professional with 11 years of experience in software
development and pre-sales, encompassing a wide range of skill sets, roles, and industry
verticals. He has solid expertise in data communication networking, software-based switching
and routing solutions, and virtualization platforms such as OpenStack, OpenDaylight
Controller, Docker containers, CoreOS, and so on. He is currently working as an advisor and
technical consultant for networking companies in the development of Layer 2 and Layer 3 IP
protocols. Kingston also works on various technologies such as MVC-based web and Windows
applications, e-commerce frameworks, open source IoT frameworks, single-board computers
such as Raspberry Pi and Intel Galileo, and so on. He is active in various networking standard
bodies such as IETF, IEEE, and ONF. He has proposed a couple of drafts in the MPLS working
group of IETF. With the current surge in SDN, virtualization, and NFV, his primary focus is in
these areas. Kingston completed a BE in computer science from Madras University.

### Suggestions and Feedback
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSdy7dATC6QmEL81FIUuymZ0Wy9vH1jHkvpY57OiMeKGqib_Ow/viewform) if you have any feedback or suggestions.


### Download a free PDF

 <i>If you have already purchased a print or Kindle version of this book, you can get a DRM-free PDF version at no cost.<br>Simply click on the link to claim your free PDF.</i>
<p align="center"> <a href="https://packt.link/free-ebook/9781783987948">https://packt.link/free-ebook/9781783987948 </a> </p>